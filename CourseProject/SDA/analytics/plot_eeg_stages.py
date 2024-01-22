import mne
import numpy
import matplotlib.pyplot as plt

def plot_eeg_stages(data: mne.io.Raw, edges: numpy.ndarray, edge_epochs: mne.Epochs, resolution: float = 1) -> plt.Figure:
    events = mne.make_fixed_length_events(data, duration = resolution)
    epochs = mne.Epochs(data, events, baseline = None, tmin = 0, tmax = resolution, preload = True, verbose = False)
    data = numpy.average(epochs.get_data(copy = True), axis = 2)

    fig, ax = plt.subplots(1, 1, figsize = (20, 5))
    ax.set_title("EEG")
    ax.set_xlabel('Time (s)')

    x = epochs.events[:, 0] / epochs.info['sfreq']
    ax.set_xlim(x[0], x[-1])
    for i in range(0, data.shape[1]): ax.plot(x, data[:, i])

    edges_sec = [ ]
    for edge in edges:
        if edge >= len(edge_epochs.events): edge -= 1
        edges_sec.append(edge_epochs.events[edge][0] / edge_epochs.info['sfreq'])
    for idx in range(len(edges_sec) - 1):
        center = (edges_sec[idx] + edges_sec[idx + 1]) / 2
        ax.text(center, numpy.max(data), idx + 1, fontsize = 9, color = 'black', horizontalalignment = 'center', fontweight = 'bold')
    ax.vlines(x = edges_sec, ymin = numpy.min(data), ymax = numpy.max(data), color = 'black', linewidth = 1.8)

    return fig