subj = "Subj1"
exp = "exp_full_flow"

import os

import numpy
import torch
import torch.utils.data
import pytorch_lightning

from RTD_AE.src.utils import *
from RTD_AE.src.rtd import RTDLoss
from RTD_AE.src.autoencoder import AutoEncoder

TRY_NUM_FEATURES = numpy.cumsum(numpy.array(range(0, 75, 3))) + 10

config = {
    "max_epochs": 100,
    "rtd_every_n_batches": 1,
    "rtd_start_epoch": 0,
    "rtd_l": 1.0, 
    "card": 50,
    "n_hidden_layers": 2,
    "hidden_dim": 1000,
    "batch_size": 64,
    "engine": "ripser",
    "is_sym": True,
    "lr": 5e-4
}

def collate_with_matrix(samples):
    indicies, data, labels = zip(*samples)
    data, labels = torch.tensor(numpy.asarray(data)), torch.tensor(numpy.asarray(labels))
    if len(data.shape) > 2:
        dist_data = torch.flatten(data, start_dim=1)
    else:
        dist_data = data
    x_dist = torch.cdist(dist_data, dist_data, p=2) / numpy.sqrt(dist_data.shape[1])
    return data, x_dist, labels

features = numpy.load(f"{subj}/{exp}/features/features.npy").astype(numpy.float32)
config['input_dim'] = features.shape[1]

ds = FromNumpyDataset(features, geodesic = False, scaler = FurthestScaler(), flatten = True, n_neighbors = 2)
val_loader = torch.utils.data.DataLoader(ds, batch_size = config["batch_size"], num_workers = 16, collate_fn = collate_with_matrix)
train_loader = torch.utils.data.DataLoader(ds, batch_size = config["batch_size"], num_workers = 16, collate_fn = collate_with_matrix, shuffle = True)

for n_components in TRY_NUM_FEATURES:
    folder = f"{subj}/{exp}/features_reduced/rtd_ae/{n_components}"
    os.makedirs(folder, exist_ok = True)

    if os.path.exists(f"{folder}/features.npy"):
        continue
    
    config['latent_dim'] = n_components

    model = AutoEncoder(
        encoder = get_linear_model(m_type = 'encoder', **config),
        decoder = get_linear_model(m_type = 'decoder', **config),
        RTDLoss = RTDLoss(dim = 1, lp = 1.0,  **config),
        MSELoss = torch.nn.MSELoss(),
        **config
    )

    trainer = pytorch_lightning.Trainer(gpus = -1, max_epochs = config['max_epochs'], log_every_n_steps = 1, num_sanity_val_steps = 0)
    trainer.fit(model, train_loader, val_loader)

    latent, labels = get_latent_representations(model, val_loader)
    numpy.save(f"{folder}/features.npy", latent)
