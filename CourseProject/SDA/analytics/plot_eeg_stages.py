import mne
import numpy
import pandas
import sklearn.cluster
import matplotlib.pyplot as plt
import matplotlib.patches as ptchs

from .. import stageprocess
from .stage_timing import stage_timing
from .edge_statistics import edge_statistics

def plot_eeg_stages(
    data: mne.io.Raw, edges: numpy.ndarray,
    epochs: mne.Epochs, features: numpy.ndarray,
    df_st_edges: pandas.DataFrame, result: dict,
    resolution: float = 1, ax = None
) -> plt.Figure:
    events = mne.make_fixed_length_events(data, duration = resolution)
    draw_epochs = mne.Epochs(data, events, baseline = None, tmin = 0, tmax = resolution, preload = True, verbose = False)
    data = numpy.average(draw_epochs.get_data(copy = True), axis = 2)
    data = data[:, data.mean(axis = 0).argsort()[::-1]] # Sort for better picture
    min, max = numpy.min(data), numpy.max(data)

    edges_sec = [ ]
    for edge in edges:
        if edge >= len(epochs.events): edge -= 1
        edges_sec.append(epochs.events[edge][0] / epochs.info['sfreq'])
    st_time_len = stage_timing(edges, epochs).iloc[1]

    if ax is None:
        fig, ax = plt.subplots(1, 1, figsize = (12.5, 4))
    ax.xaxis.set_visible(False)
    ax.set_ylim(min - 3e-6, max)
    ax.grid(axis = 'y')

    for idx, (start, end, length) in enumerate(zip(edges_sec[:-1], edges_sec[1:], st_time_len)):
        center = (start + end) / 2
        color = plt.get_cmap('Set3')(idx)
        ax.axvspan(start, end, alpha = 0.3, color = color) # Background color
        ax.add_patch(ptchs.Rectangle((start, min - 3e-6), end - start, 2e-6, edgecolor = 'black', facecolor = color, fill = True, lw = 1)) # Stage

    x = draw_epochs.events[:, 0] / draw_epochs.info['sfreq']
    ax.set_xlim(x[0], x[-1])
    for i in range(0, data.shape[1]): ax.plot(x, data[:, i])
    ax.vlines(x = edges_sec, ymin = min - 3e-6, ymax = max, color = (0.3, 0.3, 0.3), linewidth = 1.8)
    
    for idx, (start, end, length) in enumerate(zip(edges_sec[:-1], edges_sec[1:], st_time_len)):
        center = (start + end) / 2
        ax.text(center, min + 3e-7, '{}s'.format(round(length)), fontstyle = 'italic', horizontalalignment = 'center') # Length

    stats = edge_statistics(features, edges)['Silh'].to_numpy()
    for value, edge in zip(stats, edges_sec[1:]):
        ax.text(edge + 7, max - 4e-6, "{:0.2f}".format(round(value * 100) / 100), color = 'black', horizontalalignment = 'center', fontweight = 'bold') # Index
        
    st_edges_all = stageprocess.form_edges_all(df_st_edges, result['St_len_min'], result['K_nb_max'], result['N_cl_max'])
    kwargs = { 'n_clusters': result['N_stages'] - 1, 'random_state': 0, 'n_init': 10 }
    labels = sklearn.cluster.KMeans(**kwargs).fit_predict(st_edges_all)
    for i, label in enumerate(sorted(set(labels), key = list(labels).index)):
        color = plt.get_cmap('Set3')(i)
        x_sc = numpy.array([ epochs.events[e][0] / epochs.info['sfreq'] for e, l in zip(st_edges_all.flat, labels) if l == label ])
        s = numpy.array([ 5 * list(x_sc).count(x) for x in x_sc ])
        ax.scatter(x_sc, numpy.full_like(x_sc, -2e-5), s = s, color = color, zorder = 2)
