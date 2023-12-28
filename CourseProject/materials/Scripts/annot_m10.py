import numpy as np
import mne 
import pandas as pd

import pdb

print(mne.__file__)
print(mne.__version__)

from matplotlib import pyplot as plt


#path = r"m10.edf"
#path = r"m12.edf"
path = r"m11.edf"
data = mne.io.read_raw_edf(path, preload=True)
#path = r"m13_annot.fif"
#data = mne.io.read_raw_fif(path, preload=True)
print(data.info)

data_=  data.copy()
print(data_.info)

#
# manually annotate data
fig = data_.plot(scalings=dict(eeg=100e-6))
fig.canvas.key_press_event('a')
pdb.set_trace()
interactive_annot = data_.annotations
#data_.annotations.save(r'fif_for_ERP/annot/Pilot_chess_paired-annot.csv')
#data_.annotations.save(r'm10-annot_bad_blinks.csv')
data_.annotations.save(r'm11-annot.csv')
#data_.save(r'm10_annot.fif')
#'''

# check annotations

#'''
#annot = mne.read_annotations('hicha2-annot.csv')