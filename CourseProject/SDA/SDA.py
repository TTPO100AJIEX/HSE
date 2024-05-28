import typing
import joblib
import itertools

import numpy
import pandas
import scipy.stats
import scipy.sparse
import sklearn.cluster
import sklearn.preprocessing
import tqdm.contrib.itertools

from .clustquality import apply_cluster_method, cluster_metrics_noground, calc_stage_metr_noground
from .stageprocess import form_stages, form_stage_bands, form_edges_all, calc_stage_distances, merge_stages_1st_step, merge_stages_2nd_step

class SDA:
    def __init__(
        self,
        n_jobs: int = -1,
        scale: bool = False,
        verbose: bool = True,

        n_clusters_min: int = 2, n_clusters_max: int = 20, n_clusters: typing.Optional[int] = None,
        k_neighbours_min: int = 20, k_neighbours_max: int = 50, k_neighbours: typing.Optional[int] = None,
        len_st_thr: typing.List[int] = [0, 20, 40, 60],
        dist_rate: float = 0.3,

        random_state: int = 0,
        n_cl_max_thr: typing.List[int] = [10, 15, 20],
        k_neighb_max_thr: typing.List[int] = [35, 40, 45, 50],
        n_edge_clusters_min: int = 2, n_edge_clusters_max: int = 15, n_edge_clusters: typing.Optional[int] = None
    ):
        self.scale = scale
        self.n_jobs = n_jobs
        self.verbose = verbose

        self.n_clusters = n_clusters or range(n_clusters_min, n_clusters_max + 1)
        self.k_neighbours = k_neighbours or range(k_neighbours_min, k_neighbours_max + 1)
        self.len_st_thr = len_st_thr
        self.dist_rate = dist_rate
        
        self.random_state = random_state
        self.n_cl_max_thr = n_cl_max_thr
        self.k_neighb_max_thr = k_neighb_max_thr
        self.n_edge_clusters = n_edge_clusters or range(n_edge_clusters_min, n_edge_clusters_max + 1)

    def apply(self, features: numpy.ndarray, df_st_edges: typing.Optional[pandas.DataFrame] = None):
        if self.scale: features = sklearn.preprocessing.StandardScaler().fit_transform(features)
        if self.verbose: print('Applying to {} samples with {} features each'.format(*features.shape))
        if df_st_edges is None: df_st_edges = self.stage1(features)
        result = self.stage2(features, df_st_edges)
        return result, df_st_edges

    def stage1_iter(self, features: numpy.ndarray, n_clusters: int, k_neighbours: int) -> pandas.DataFrame:
        n_samples, _ = features.shape
        diag_nums = numpy.arange(-k_neighbours, k_neighbours + 1)
        diag_values = numpy.ones_like(diag_nums)
        connectivity = scipy.sparse.diags(diag_values, diag_nums, (n_samples, n_samples), 'csr', numpy.int8)

        kwargs = { 'n_clusters': n_clusters, 'linkage': 'ward', 'connectivity': connectivity }
        _, labels, metrics = apply_cluster_method(features, sklearn.cluster.AgglomerativeClustering, **kwargs)

        report = { 'N_clusters': n_clusters, 'K_neighb': k_neighbours, **metrics }
        st_edges = form_stages(labels) # Forming stages from clusters

        # Merging stages
        df_st_edges = [ ]
        for len_min in self.len_st_thr:
            st_edges = merge_stages_1st_step(features, st_edges, len_min)
            st_edges = merge_stages_2nd_step(features, st_edges, self.dist_rate)
            df_st_edges.append({ **report, 'Len_min': len_min, 'St_edges': st_edges })
        return df_st_edges

    def stage1(self, features: numpy.ndarray) -> pandas.DataFrame:
        if self.verbose:
            print('Running stage 1')
            loop_over = tqdm.contrib.itertools.product(self.n_clusters, self.k_neighbours)
        else:
            loop_over = itertools.product(self.n_clusters, self.k_neighbours)
            
        df_st_edges = joblib.Parallel(n_jobs = self.n_jobs)(
            joblib.delayed(self.stage1_iter)(features, *params)
            for params in loop_over
        )
        return pandas.DataFrame(list(itertools.chain(*df_st_edges)))
    
    def stage2_iter(
        self,
        features: numpy.ndarray,
        df_st_edges: pandas.DataFrame,
        st_len: int,
        k_nb_max: int,
        n_cl: int,
        n_edge_clusters: int
    ) -> pandas.DataFrame:
        n_samples, _ = features.shape
        part_report = { 'St_len_min': st_len, 'K_nb_max': k_nb_max, 'N_cl_max': n_cl, 'N_stages': n_edge_clusters + 1 }

        # Clustering stage edges
        st_edges_all = form_edges_all(df_st_edges, st_len, k_nb_max, n_cl)
        kwargs = { 'n_clusters': n_edge_clusters, 'random_state': self.random_state, 'n_init': 10 }
        _, labels, _ = apply_cluster_method(st_edges_all, sklearn.cluster.KMeans, **kwargs)
        
        # Form stages by centers of clusters (median, mean, mode)
        st_medians = []
        st_modes = []
        st_means = []
        for _st in range(n_edge_clusters):
            st_cluster = st_edges_all[numpy.where(labels == _st)[0]]
            if len(st_cluster) == 0:
                continue
            st_modes.append(int(scipy.stats.mode(st_cluster, nan_policy = 'omit').mode))
            st_medians.append(int(numpy.median(st_cluster)))
            st_means.append(int(numpy.mean(st_cluster)))

        result = [ ]
        for (st_centers, cl_center_type) in zip((st_medians, st_modes, st_means), ('Median', 'Mode', 'Mean')):
            st_edges_centers = [0] + sorted(st_centers) + [n_samples]
            report = { **part_report, 'Cl_cen': cl_center_type, 'St_edges': st_edges_centers }
            _, new_labels = form_stage_bands(st_edges_centers)

            st_dist_ward, st_dist_centr = numpy.mean(calc_stage_distances(features, st_edges_centers), axis = 1)
            overall_metrics = cluster_metrics_noground(features, new_labels)
                
            # Clustering metrics for pairs of adjacent stages
            df_avg_metrics = calc_stage_metr_noground(features, st_edges_centers).mean()
            avg_metrics = df_avg_metrics.rename(lambda col: f'Avg-{col}').to_dict()

            result.append({ **report, 'Ward_dist': st_dist_ward, 'Cen_dist': st_dist_centr, **overall_metrics, **avg_metrics })
        return result

    def stage2(self, features: numpy.ndarray, df_st_edges: pandas.DataFrame) -> pandas.DataFrame:
        if self.verbose:
            print('Running stage 2')
            loop_over = tqdm.contrib.itertools.product(self.len_st_thr, self.k_neighb_max_thr, self.n_cl_max_thr, self.n_edge_clusters)
        else:
            loop_over = itertools.product(self.len_st_thr, self.k_neighb_max_thr, self.n_cl_max_thr, self.n_edge_clusters)
            
        result = joblib.Parallel(n_jobs = self.n_jobs)(
            joblib.delayed(self.stage2_iter)(features, df_st_edges, *params)
            for params in loop_over
        )
        return pandas.DataFrame(list(itertools.chain(*result)))
