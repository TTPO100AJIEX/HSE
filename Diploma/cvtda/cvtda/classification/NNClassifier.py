import numpy
import torch
import sklearn.base
import torch.utils.data

from cvtda.utils import set_random_seed
from cvtda.logging import DevNullLogger, CLILogger

class NNClassifier(sklearn.base.ClassifierMixin):
    fitted_ = False

    def __init__(
        self,
        verbose: bool = True,
        random_state: int = 42,

        device: torch.device = torch.device("cuda"),
        batch_size: int = 1024,
        learning_rate: float = 5e-4,
        n_epochs: int = 50
    ):
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.device_ = device
        self.batch_size_ = batch_size
        self.learning_rate_ = learning_rate
        self.n_epochs_ = n_epochs

    
    def fit(self, features: numpy.ndarray, y_true: numpy.ndarray):
        set_random_seed(self.random_state_)

        train_ds = torch.utils.data.TensorDataset(
            torch.tensor(features, dtype = torch.float),
            torch.tensor(y_true, dtype = torch.long)
        )
        train_dl = torch.utils.data.DataLoader(
            train_ds,
            batch_size = self.batch_size_,
            shuffle = True
        )

        self.model_ = torch.nn.Sequential(
            torch.nn.Linear(features.shape[1], 128), torch.nn.BatchNorm1d(128), torch.nn.GELU(),
            torch.nn.Linear(128, 64), torch.nn.BatchNorm1d(64), torch.nn.GELU(),
            torch.nn.Linear(64, 32), torch.nn.BatchNorm1d(32), torch.nn.GELU(),
            torch.nn.Linear(32, int(numpy.max(y_true)) + 1), torch.nn.Softmax(dim = 1)
        ).to(self.device_).eval()

        self.optimizer_ = torch.optim.AdamW(
            params = self.model_.parameters(),
            lr = self.learning_rate_
        )
        
        pbar = self.logger_.loop(range(self.n_epochs_))
        for _ in pbar:
            sum_loss = 0

            for (X, y) in train_dl:
                self.optimizer_.zero_grad()
                pred = self.model_(X.to(self.device_))
                loss = torch.nn.functional.cross_entropy(pred, y.to(self.device_), reduction = 'sum')
                loss.backward()
                self.optimizer_.step()
                sum_loss += loss.item()

            self.logger_.set_pbar_postfix(pbar, { 'loss': sum_loss })

        self.fitted_ = True
        return self
    
    def predict_proba(self, features: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before predict_proba()'
        set_random_seed(self.random_state_)
        self.model_.eval()

        test_dl = torch.utils.data.DataLoader(
            torch.tensor(features, dtype = torch.float),
            batch_size = self.batch_size_,
            shuffle = False
        )

        y_pred_proba = [ ]
        with torch.no_grad():
            for X in test_dl:
                y_pred_proba.append(self.model_(X.to(self.device_)))
        return torch.vstack(y_pred_proba).cpu().numpy()
