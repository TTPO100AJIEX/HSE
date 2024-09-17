
import os
import random

import tqdm
import torch
import numpy
import pandas

subj = "Subj1"
exp = "exp_full_flow"
TRY_NUM_FEATURES = numpy.cumsum(numpy.array(range(0, 75, 3))) + 10

device = torch.device(
    "cuda" if torch.cuda.is_available()
    else "mps" if torch.backends.mps.is_available()
    else "cpu"
)

def set_random_seed(seed: int):
    random.seed(seed)
    numpy.random.seed(seed)
    torch.manual_seed(seed)
    torch.cuda.manual_seed(seed)
    os.environ["PYTHONHASHSEED"] = str(seed)
    torch.backends.cudnn.deterministic = True
    
class AutoEncoder:
    def __init__(
        self,
        n_features: int,
        n_components: int,

        learning_rate: float = 1e-4,
        n_epochs: int = 300,
        random_state: int = 42
    ):
        self.encoder = torch.nn.Sequential(
            torch.nn.Linear(in_features = n_features, out_features = 4000), torch.nn.BatchNorm1d(4000), torch.nn.GELU(),
            torch.nn.Linear(in_features = 4000, out_features = 1000), torch.nn.BatchNorm1d(1000), torch.nn.GELU(),
            torch.nn.Linear(in_features = 1000, out_features = n_components)
        ).to(device)
        self.decoder = torch.nn.Linear(in_features = n_components, out_features = n_features).to(device)
        
        self.n_components = n_components
        self.learning_rate = learning_rate
        self.n_epochs = n_epochs
        self.random_state = random_state
    
    def fit(self, data: torch.Tensor):
        def lr_scheduler(epoch: int):
            if epoch < self.n_epochs * 1 // 3: return 1
            if epoch < self.n_epochs * 2 // 3: return 0.1
            return 0.01

        optimizer = torch.optim.AdamW(
            list(self.encoder.parameters()) + list(self.decoder.parameters()),
            lr = self.learning_rate
        )
        scheduler = torch.optim.lr_scheduler.LambdaLR(optimizer, lr_scheduler)
        
        loss_history = [ ]
        self.encoder.train()
        self.decoder.train()
        for _ in tqdm.trange(self.n_epochs, desc = f"{self.n_components}"):
            optimizer.zero_grad()
            encoded = self.encoder(data)
            decoded = self.decoder(encoded)

            loss = torch.nn.functional.mse_loss(decoded, data, reduction = 'sum')
            loss.backward()
            optimizer.step()

            loss_history.append(loss.item())
            scheduler.step()
    
    def fit_transform(self, features: numpy.ndarray):
        set_random_seed(self.random_state)
        data = torch.Tensor(features).to(device)
        self.fit(data)
        self.encoder.eval()
        return self.encoder(data).detach().cpu().numpy()
    
features = pandas.read_feather(f"{subj}/{exp}/features/features.feather").to_numpy()
for n_components in TRY_NUM_FEATURES:
    folder = f"{subj}/{exp}/features_reduced/ae/{n_components}"
    os.makedirs(folder, exist_ok = True)

    if os.path.exists(f"{folder}/features.npy"):
        continue

    method = AutoEncoder(n_features = features.shape[1], n_components = n_components)
    numpy.save(f"{folder}/features.npy", method.fit_transform(features))
