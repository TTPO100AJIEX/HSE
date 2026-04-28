import numpy as np
import sklearn.neighbors
import gudhi


def cubical_pd(scalar_field: np.ndarray) -> np.ndarray:
    """
    Sublevel-set persistence diagram of a 2D scalar field via cubical complex.

    Returns array shape (n_pts, 3): columns [dimension, birth, death].
    Infinite-death points (the global maximum component) are dropped.
    """
    cc = gudhi.CubicalComplex(
        dimensions=list(scalar_field.shape),
        top_dimensional_cells=scalar_field.flatten().tolist(),
    )
    cc.compute_persistence()
    pairs = cc.persistence()
    finite = [
        (float(dim), float(b), float(d))
        for dim, (b, d) in pairs
        if np.isfinite(d)
    ]
    if not finite:
        return np.zeros((0, 3), dtype=np.float32)
    return np.array(finite, dtype=np.float32)


def feature_map_to_pd(feature_map) -> np.ndarray:
    """
    Persistence diagram from a CNN feature map.

    feature_map: torch.Tensor or np.ndarray of shape (C, H, W).
    Reduces channels via L2 norm to a scalar field (H, W), then
    runs sublevel-set cubical persistence.
    """
    if hasattr(feature_map, "detach"):
        arr = feature_map.detach().cpu().numpy()
    else:
        arr = np.asarray(feature_map, dtype=np.float32)
    scalar_field = np.linalg.norm(arr, axis=0).astype(np.float32)  # (H, W)
    return cubical_pd(scalar_field)


# ---------------------------------------------------------------------------
# Internals: vectorization
# ---------------------------------------------------------------------------

def _pd_to_vec(
    pd: np.ndarray,
    resolution: int,
    max_val: float,
    hom_dims: tuple = (0, 1),
) -> np.ndarray:
    """Vectorize a single PD as a flat persistence-image histogram."""
    vecs = []
    for dim in hom_dims:
        grid = np.zeros(resolution * resolution, dtype=np.float32)
        if len(pd) > 0:
            pts = pd[pd[:, 0] == dim]
            if len(pts) > 0:
                births = pts[:, 1]
                pers = pts[:, 2] - pts[:, 1]
                bi = np.clip((births / max_val * resolution).astype(int), 0, resolution - 1)
                pi = np.clip((pers / max_val * resolution).astype(int), 0, resolution - 1)
                np.add.at(grid, bi * resolution + pi, 1.0)
        vecs.append(grid)
    return np.concatenate(vecs)


def _vectorize_pi(
    pds: list,
    resolution: int = 20,
    hom_dims: tuple = (0, 1),
) -> np.ndarray:
    """Vectorize a list of PDs as persistence-image histograms."""
    all_vals = []
    for pd in pds:
        if len(pd) > 0:
            all_vals.extend(pd[:, 1].tolist())
            all_vals.extend((pd[:, 2] - pd[:, 1]).tolist())

    if not all_vals:
        max_val = 1.0
    else:
        max_val = float(np.percentile(all_vals, 95))
        if max_val == 0.0:
            max_val = float(np.max(all_vals)) if np.max(all_vals) > 0 else 1.0

    return np.array(
        [_pd_to_vec(pd, resolution, max_val, hom_dims) for pd in pds],
        dtype=np.float32,
    )


# ---------------------------------------------------------------------------
# Internals: metric-based k-NN (Wasserstein / bottleneck)
# ---------------------------------------------------------------------------

def _extract_dim(pd: np.ndarray, dim: int) -> np.ndarray:
    """Return (birth, death) pairs for a given homological dimension."""
    if len(pd) == 0:
        return np.zeros((0, 2), dtype=np.float64)
    pts = pd[pd[:, 0] == dim][:, 1:]
    return pts.astype(np.float64) if len(pts) > 0 else np.zeros((0, 2), dtype=np.float64)


def _metric_knn(
    pds: list,
    k: int,
    method: str = "wasserstein",
    hom_dim: int = 1,
) -> "scipy.sparse.csr_matrix":
    """
    Build a k-NN graph via direct Wasserstein or bottleneck distance.

    For N > ~500 images this becomes expensive; consider subsampling upstream.
    """
    import scipy.sparse
    import gudhi.wasserstein

    diags = [_extract_dim(pd, hom_dim) for pd in pds]
    N = len(diags)
    dist = np.zeros((N, N), dtype=np.float64)

    for i in range(N):
        for j in range(i + 1, N):
            if method == "wasserstein":
                d_ij = gudhi.wasserstein.wasserstein_distance(
                    diags[i], diags[j], order=1.0
                )
            else:  # bottleneck
                d_ij = gudhi.bottleneck_distance(diags[i], diags[j])
            dist[i, j] = dist[j, i] = d_ij

    rows, cols = [], []
    for i in range(N):
        neighbors = np.argsort(dist[i])[1 : k + 1]
        rows.extend([i] * len(neighbors))
        cols.extend(neighbors.tolist())

    return scipy.sparse.csr_matrix(
        (np.ones(len(rows)), (rows, cols)), shape=(N, N)
    )


# ---------------------------------------------------------------------------
# Public API
# ---------------------------------------------------------------------------

def pds_to_knn_graph(pds: list, k: int, method: str = "pi", **kwargs):
    """
    Build a k-NN graph on a population of persistence diagrams.

    pds    : list of N arrays, each shape (n_pts, 3) — [dim, birth, death]
    k      : number of neighbors per vertex
    method : 'pi'          — persistence-image vectorization + Euclidean k-NN (default)
             'wasserstein' — pairwise Wasserstein-1 distance, O(N²) per layer
             'bottleneck'  — pairwise bottleneck distance,  O(N²) per layer

    Returns scipy.sparse CSR matrix of shape (N, N).
    """
    if method == "pi":
        resolution = kwargs.get("resolution", 20)
        hom_dims = kwargs.get("hom_dims", (0, 1))
        vecs = _vectorize_pi(pds, resolution=resolution, hom_dims=hom_dims)
        return sklearn.neighbors.kneighbors_graph(
            vecs, n_neighbors=k, mode="connectivity"
        )
    elif method in ("wasserstein", "bottleneck"):
        hom_dim = kwargs.get("hom_dim", 1)
        return _metric_knn(pds, k=k, method=method, hom_dim=hom_dim)
    else:
        raise ValueError(
            f"Unknown method {method!r}. Choose 'pi', 'wasserstein', or 'bottleneck'."
        )
