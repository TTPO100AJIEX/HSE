import typing

import torch
import torch.utils.data
import pytorch_metric_learning.miners
import pytorch_metric_learning.losses
import pytorch_metric_learning.samplers
import pytorch_metric_learning.utils.accuracy_calculator

import cvtda.utils
import cvtda.logging
import cvtda.neural_network

from .BaseLearner import BaseLearner


class NNLearner(BaseLearner):
    def __init__(
        self,

        n_jobs: int = -1,
        random_state: int = 42,

        device: torch.device = torch.device("cuda"),
        batch_size: int = 64,
        learning_rate: float = 1e-4,
        n_epochs: int = 25,

        margin: int = 1,
        latent_dim: int = 256,
        
        skip_diagrams: bool = False,
        skip_images: bool = False,
        skip_features: bool = False
    ):
        super().__init__(n_jobs)
        self.random_state_ = random_state

        self.device_ = device
        self.batch_size_ = batch_size
        self.learning_rate_ = learning_rate
        self.n_epochs_ = n_epochs

        self.margin_ = margin
        self.latent_dim_ = latent_dim

        self.skip_diagrams_ = skip_diagrams
        self.skip_images_ = skip_images
        self.skip_features_ = skip_features


    def fit(self, train: cvtda.neural_network.Dataset, val: typing.Optional[cvtda.neural_network.Dataset]):
        cvtda.utils.set_random_seed(self.random_state_)

        train_mpc_sampler = pytorch_metric_learning.samplers.MPerClassSampler(
            m = 4,
            labels = train.labels,
            length_before_new_iter = self.batch_size_ * 20
        )
        train_dl = torch.utils.data.DataLoader(
            torch.utils.data.TensorDataset(torch.arange(len(train))),
            batch_size = self.batch_size_,
            sampler = train_mpc_sampler
        )
        self.init_(next(iter(train_dl)), train)

        train_miner = pytorch_metric_learning.miners.TripletMarginMiner(
            margin = self.margin_, 
            type_of_triplets = "all"
        )
        train_loss = pytorch_metric_learning.losses.TripletMarginLoss(margin = self.margin_)
        metrics = pytorch_metric_learning.utils.accuracy_calculator.AccuracyCalculator()
        
        if val is not None:
            val_dl = torch.utils.data.DataLoader(
                torch.utils.data.TensorDataset(torch.arange(len(val))),
                batch_size = self.batch_size_
            )

        pbar = cvtda.logging.logger().pbar(range(self.n_epochs_), desc = "Train")
        for _ in pbar:
            sum_loss = 0

            self.model_list_.train()
            for idxs in train_dl:
                self.optimizer_.zero_grad()
                targets = train.get_labels(idxs)
                embeddings = self.forward_(idxs, train)
                indices = train_miner(embeddings, targets)
                loss = train_loss(embeddings, targets, indices)
                loss.backward()
                self.optimizer_.step()
                sum_loss += loss.item()
            postfix = { 'loss': sum_loss }

            if val is not None:
                self.model_list_.eval()
                all_embeddings, all_targets = [], []
                for idxs in val_dl:
                    with torch.no_grad():
                        all_targets.append(val.get_labels(idxs))
                        all_embeddings.append(self.forward_(idxs, val))
                result = metrics.get_accuracy(torch.cat(all_embeddings, dim = 0), torch.cat(all_targets, dim = 0))
                postfix = { **postfix, **result }

            cvtda.logging.logger().set_pbar_postfix(pbar, postfix)

        self.model_list_.eval()
        self.fitted_ = True
        return self
    
    def calculate_distance_(self, first: int, second: int, dataset: cvtda.neural_network.Dataset):
        self.model_list_.eval()
        embeddings = self.forward_([ first, second ], dataset)
        return torch.sqrt(torch.sum((embeddings[0] - embeddings[1]) ** 2)).item()


    def init_(self, idxs: torch.Tensor, dataset: cvtda.neural_network.Dataset):
        images, _, *diagrams = dataset.get_features(idxs, skip_diagrams = self.skip_diagrams_)
        
        self.nn_base_ = cvtda.neural_network.NNBase(
            num_diagrams = len(diagrams) // 2,
            skip_diagrams = self.skip_diagrams_,
            skip_images = self.skip_images_,
            skip_features = self.skip_features_,
            images_n_channels = images.shape[1]
        ).to(self.device_).train()

        self.model_ = torch.nn.Sequential(
            torch.nn.Dropout(0.3), torch.nn.LazyLinear(1024), torch.nn.BatchNorm1d(1024), torch.nn.GELU(),
            torch.nn.Dropout(0.2), torch.nn.Linear(1024, 768), torch.nn.BatchNorm1d(768), torch.nn.GELU(),
            torch.nn.Dropout(0.1), torch.nn.Linear(768, 512), torch.nn.BatchNorm1d(512), torch.nn.GELU(),
            torch.nn.Linear(512, self.latent_dim_)
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
        return self.model_(self.nn_base_(*dataset.get_features(idxs, skip_diagrams = self.skip_diagrams_)))
