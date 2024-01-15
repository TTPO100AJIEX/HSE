import mne
import numpy
import matplotlib.pyplot as plt
import matplotlib.patches as ptchs

from .. import stageprocess
from .stage_timing import stage_timing
from .edge_statistics import edge_statistics

def plot_stats(features: numpy.ndarray, edges: numpy.ndarray, epochs: mne.Epochs):
    plt.xlim(0, len(features))
    ymin, ymax = -0.2, 1.1
    plt.ylim(ymin, ymax)
    plt.grid(axis = 'y')
    plt.xlabel('Epoches')
    plt.tick_params(axis = 'both', labelsize = 11, direction = 'in')
    plt.vlines(edges[1:-1], ymin, ymax, color = 'black', linewidth = 1) # Stage boundaries
    
    st_time_len = stage_timing(edges, epochs).iloc[1]
    st_bands, _ = stageprocess.form_stage_bands(edges)
    for i, ((smin, smax), length) in enumerate(zip(st_bands, st_time_len)):
        center = (smin + smax) / 2
        color = plt.get_cmap('Set3')(i)
        plt.axvspan(smin, smax, alpha = 0.3, color = color) # Background color
        plt.text(center, -0.12, i, fontsize = 10, fontweight = 'bold', horizontalalignment = 'center') # Name
        plt.text(center, 0.02, '{}s'.format(round(length)), fontsize = 9, fontstyle = 'italic', horizontalalignment = 'center') # Length
        plt.gca().add_patch(ptchs.Rectangle((smin, -0.05), smax - smin, 0.05, edgecolor = 'black', facecolor = color, fill = True, lw = 1)) # Stage

    stats = edge_statistics(features, edges)
    for idx, column in enumerate(stats):
        color = plt.get_cmap('Set1')(idx)
        stats[column] /= stats[column].max()
        plt.plot(edges[1:-1], stats[column], linestyle = '--', marker = 'o', color = color, label = column)
    plt.legend()