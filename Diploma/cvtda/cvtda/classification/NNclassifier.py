import typing

import numpy
import torch
import sklearn.base
import sklearn.metrics
import torch.utils.data

import cvtda.utils
import cvtda.logging
import cvtda.neural_network


class NNClassifier(sklearn.base.ClassifierMixin):
    def __init__(
        self,

        n_jobs: int = -1,
        random_state: int = 42,

        device: torch.device = torch.device("cuda"),
        batch_size: int = 128,
        learning_rate: float = 1e-4,
        n_epochs: int = 25
    ):
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.device_ = device
        self.batch_size_ = batch_size
        self.learning_rate_ = learning_rate
        self.n_epochs_ = n_epochs


    def fit(self, train: Dataset, val: Dataset):
        set_random_seed(self.random_state_)
        train_dl = self.make_dataloader_(train_features, train_labels, shuffle = True)
        self.init_(next(iter(train_dl))[0], int(numpy.max(train_labels)) + 1)
        
        pbar = self.logger_.pbar(range(self.n_epochs_))
        for _ in pbar:
            sum_loss = 0

            self.model_.train()
            for (X, y) in train_dl:
                self.optimizer_.zero_grad()
                pred = self.forward_(X)
                loss = torch.nn.functional.cross_entropy(pred, y.to(self.device_), reduction = 'sum')
                loss.backward()
                self.optimizer_.step()
                sum_loss += loss.item()
            postfix = { 'loss': sum_loss }

            if (val_features is not None) and (val_labels is not None):
                val_proba = self.predict_proba_(val_features)
                val_pred = numpy.argmax(val_proba, axis = 1)
                postfix['val_acc'] = sklearn.metrics.accuracy_score(val_labels, val_pred)

            self.logger_.set_pbar_postfix(pbar, postfix)

        self.fitted_ = True
        return self
    
    def predict_proba(self, features: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before predict_proba()'
        set_random_seed(self.random_state_)
        return self.predict_proba_(features)


    def init_(self, example_features: torch.Tensor, num_labels: int):
        self.model_ = torch.nn.Sequential(
            torch.nn.Dropout(0.4), torch.nn.LazyLinear(256), torch.nn.BatchNorm1d(256), torch.nn.ReLU(),
            torch.nn.Dropout(0.3), torch.nn.Linear(256, 128), torch.nn.BatchNorm1d(128), torch.nn.ReLU(),
            torch.nn.Dropout(0.2), torch.nn.Linear(128, 64), torch.nn.BatchNorm1d(64), torch.nn.ReLU(),
            torch.nn.Dropout(0.1), torch.nn.Linear(64, 32), torch.nn.BatchNorm1d(32), torch.nn.ReLU(),
            torch.nn.Linear(32, num_labels), torch.nn.Softmax(dim = 1)
        ).to(self.device_).train()

        self.optimizer_ = torch.optim.AdamW(
            params = self.model_.parameters(),
            lr = self.learning_rate_
        )

        self.forward_(example_features)
        self.logger_.print(f'Input to LazyLinear: {self.model_[1].in_features}')
        self.logger_.print(f'Parameters: {sum(p.numel() for p in self.model_.parameters())}')

    def forward_(self, X: numpy.ndarray) -> numpy.ndarray:
        X = X.to(self.device_)
        X = torch.squeeze(X)
        return self.model_(X)

    def make_dataloader_(
        self, features: numpy.ndarray, labels: typing.Optional[numpy.ndarray] = None, shuffle: bool = False
    ) -> torch.utils.data.DataLoader:
        features = numpy.expand_dims(features, axis = 1)
        features = torch.tensor(features, dtype = torch.float)
        if labels is not None:
            labels = torch.tensor(labels, dtype = torch.long)

        if labels is None:
            train_ds = features
        else:
            train_ds = torch.utils.data.TensorDataset(features, labels)
        return torch.utils.data.DataLoader(train_ds, batch_size = self.batch_size_, shuffle = shuffle)
    
    def predict_proba_(self, features: numpy.ndarray) -> numpy.ndarray:
        y_pred_proba = [ ]
        self.model_.eval()
        with torch.no_grad():
            for X in self.make_dataloader_(features, shuffle = False):
                y_pred_proba.append(self.forward_(X))
        return torch.vstack(y_pred_proba).cpu().numpy()
