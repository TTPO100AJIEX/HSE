import torch
import sys
sys.path.append("../")
import numpy as np
import matplotlib.pyplot as plt
import tempfile
from pathlib import Path
from typing import List, Tuple, Optional, Dict, Any
from dataclasses import dataclass

# Core imports
from transformers import AutoTokenizer, AutoModelForCausalLM
from tqdm import tqdm

# Visualization imports
from matplotlib.collections import LineCollection
from matplotlib.cm import ScalarMappable
from matplotlib.colors import Normalize, ListedColormap
import matplotlib.patches as mpatches

# Analysis imports
from sklearn.manifold import TSNE
from sklearn.decomposition import PCA
from sklearn.neighbors import NearestNeighbors

# Topological analysis imports
import dionysus as d
from .zigzag_DL import ZIGZAG

class TopologicalAnalyzer:
    """Handles topological data analysis using persistent homology."""
    
    @staticmethod
    def convert_diagrams_to_numpy(diagrams):
        """Convert dionysus diagrams to numpy arrays."""
        return [
            np.array([[interval.birth, interval.death] for interval in diag]) 
            for diag in diagrams
        ]
    
    @staticmethod
    def compute_zigzag_barcodes(representations: torch.Tensor, 
                               params: Optional[Dict] = None, 
                               show_plots: bool = False, 
                               save_plots: bool = False, 
                               output_dir: Optional[str] = None) -> Dict[str, Any]:
        """Compute zigzag persistent homology barcodes."""
        if params is None:
            params = {"knn": 2, "dim": 3}
        
        token_array = representations.cpu().numpy()
        zigclass = ZIGZAG(params, reps=token_array)
        
        # Generate simplicial complex and compute persistence
        simplices, simplices_padded = zigclass.generate_simplex_tree()
        layers = zigclass.compute_layers_with_intersection(simplices_padded)
        filtration, times = zigclass.compute_filtration_times(simplices, layers)
        zz, diagrams, cells = zigclass.compute_zigzag_persistence(filtration, times)
        
        # Convert to different formats
        diagrams_numpy = TopologicalAnalyzer.convert_diagrams_to_numpy(diagrams)
        converted_diagrams = [np.array(dgm) // 2 for dgm in diagrams_numpy]
        dionysus_diagrams = [d.Diagram(dgm) for dgm in converted_diagrams]
        
        return {
            'diagrams': dionysus_diagrams,
            'raw_diagrams': diagrams_numpy,
            'converted_diagrams': converted_diagrams,
            'simplices': simplices,
            'layers': layers,
            'filtration': filtration,
            'times': times
        }

    @staticmethod
    def compute_zigzag_barcodes_from_knn_graphs(
        knn_graphs: list,
        params: Optional[Dict] = None,
    ) -> Dict[str, Any]:
        """Compute zigzag persistent homology barcodes from precomputed k-NN graphs.

        knn_graphs: list of L sparse matrices (one per layer), each shape (N, N).
        params:     dict with keys 'dim' (max simplex dimension, default 3).
        """
        if params is None:
            params = {"knn": 4, "dim": 3}

        zigclass = ZIGZAG(params)
        simplices, simplices_padded = zigclass.generate_simplex_tree(knn_graphs=knn_graphs)
        layers = zigclass.compute_layers_with_intersection(simplices_padded)
        filtration, times = zigclass.compute_filtration_times(simplices, layers)
        zz, diagrams, cells = zigclass.compute_zigzag_persistence(filtration, times)

        diagrams_numpy = TopologicalAnalyzer.convert_diagrams_to_numpy(diagrams)
        converted_diagrams = [np.array(dgm) // 2 for dgm in diagrams_numpy]
        dionysus_diagrams = [d.Diagram(dgm) for dgm in converted_diagrams]

        return {
            'diagrams': dionysus_diagrams,
            'raw_diagrams': diagrams_numpy,
            'converted_diagrams': converted_diagrams,
            'simplices': simplices,
            'layers': layers,
            'filtration': filtration,
            'times': times,
        }