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
        n_epochs: int = 25,
        
        skip_diagrams: bool = False,
        skip_images: bool = False,
        skip_features: bool = False
    ):
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state

        self.device_ = device
        self.batch_size_ = batch_size
        self.learning_rate_ = learning_rate
        self.n_epochs_ = n_epochs

        self.skip_diagrams_ = skip_diagrams
        self.skip_images_ = skip_images
        self.skip_features_ = skip_features


    def fit(self, train: cvtda.neural_network.Dataset, val: typing.Optional[cvtda.neural_network.Dataset]):
        cvtda.utils.set_random_seed(self.random_state_)
        train_dl = torch.utils.data.DataLoader(
            torch.utils.data.TensorDataset(torch.arange(len(train))),
            batch_size = self.batch_size_,
            shuffle = True
        )
        self.init_(next(iter(train_dl)), train)

        for epoch in range(self.n_epochs_):
            sum_loss = 0

            self.model_list_.train()
            for idxs in cvtda.logging.logger().pbar(train_dl, desc = f"Epoch {epoch}"):
                self.optimizer_.zero_grad()
                pred = self.forward_(idxs, train)
                loss = torch.nn.functional.cross_entropy(pred, train.get_labels(idxs), reduction = 'sum')
                loss.backward()
                self.optimizer_.step()
                sum_loss += loss.item()
            postfix = { 'loss': sum_loss }

            if val is not None:
                val_proba = self.predict_proba_(val)
                val_pred = numpy.argmax(val_proba, axis = 1)
                postfix['val_acc'] = sklearn.metrics.accuracy_score(val.labels, val_pred)

            print(f"Epoch {epoch}:", postfix)

        self.fitted_ = True
        return self
    
    def predict_proba(self, dataset: cvtda.neural_network.Dataset) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before predict_proba()'
        cvtda.utils.set_random_seed(self.random_state_)
        return self.predict_proba_(dataset)


    def init_(self, idxs: torch.Tensor, dataset: cvtda.neural_network.Dataset):
        images, _, *diagrams = dataset.get_features(idxs)
        labels = dataset.get_labels(idxs)
        
        self.nn_base_ = cvtda.neural_network.NNBase(
            num_diagrams = len(diagrams) // 2,
            skip_diagrams = self.skip_diagrams_,
            skip_images = self.skip_images_,
            skip_features = self.skip_features_,
            images_n_channels = images.shape[1]
        ).to(self.device_).train()

        self.model_ = torch.nn.Sequential(
            torch.nn.Dropout(0.4), torch.nn.LazyLinear(256), torch.nn.BatchNorm1d(256), torch.nn.ReLU(),
            torch.nn.Dropout(0.3), torch.nn.Linear(256, 128), torch.nn.BatchNorm1d(128), torch.nn.ReLU(),
            torch.nn.Dropout(0.2), torch.nn.Linear(128, 64), torch.nn.BatchNorm1d(64), torch.nn.ReLU(),
            torch.nn.Dropout(0.1), torch.nn.Linear(64, 32), torch.nn.BatchNorm1d(32), torch.nn.ReLU(),
            torch.nn.Linear(32, len(torch.unique(labels))), torch.nn.Softmax(dim = 1)
        ).to(self.device_).train()

        self.model_list_ = torch.nn.ModuleList([ self.nn_base_, self.model_ ])

        self.optimizer_ = torch.optim.AdamW(
            params = self.model_list_.parameters(),
            lr = self.learning_rate_
        )

        self.forward_(idxs, dataset)
        cvtda.logging.logger().print(f'Input to LazyLinear: {self.model_[1].in_features}')
        cvtda.logging.logger().print(f'Parameters: {sum(p.numel() for p in self.model_list_.parameters())}')

    def forward_(self, idxs: torch.Tensor, dataset: cvtda.neural_network.Dataset) -> torch.Tensor:
        return self.model_(self.nn_base_(*dataset.get_features(idxs)))

    def predict_proba_(self, dataset: cvtda.neural_network.Dataset) -> numpy.ndarray:
        dl = torch.utils.data.DataLoader(
            torch.utils.data.TensorDataset(torch.arange(len(dataset))),
            batch_size = self.batch_size_,
            shuffle = False
        )

        y_pred_proba = [ ]
        self.model_list_.eval()
        with torch.no_grad():
            for idxs in dl:
                y_pred_proba.append(self.forward_(idxs, dataset))
        return torch.vstack(y_pred_proba).cpu().numpy()
