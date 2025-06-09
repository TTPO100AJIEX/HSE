from .Dataset import Dataset
from .NNBase import NNBase

import torch
if torch.cuda.is_available():
    default_device = torch.device('cuda')
elif torch.mps.is_available():
    default_device = torch.device('mps')
else:
    default_device = torch.device('cpu')
