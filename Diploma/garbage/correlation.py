import numpy
import sklearn.base
import matplotlib.pyplot as plt

from cvtda.logging import DevNullLogger, CLILogger

def correlate_with_target(features: numpy.ndarray, target: numpy.ndarray) -> numpy.ndarray:
    full_matrix = numpy.hstack([ features, target.reshape(-1, 1) ])
    correlations = numpy.corrcoef(full_matrix, rowvar = False)[-1, :-1]
    return numpy.nan_to_num(correlations, nan = -1)

class CorrelationSelector(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,

        threshold: float = 0.5
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.threshold_ = threshold

    def fit(self, features: numpy.ndarray, target: numpy.ndarray):
        self.logger_.print('Fitting the correlations feature selector')

        self.correlations_ = correlate_with_target(features, target)
        self.good_features_idx_ = numpy.nonzero(self.correlations_ >= self.threshold_)[0]
        
        self.logger_.print('Fitting complete')
        self.fitted_ = True
        return self
    
    def transform(self, features: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        return features[:, self.good_features_idx_]

    def hist(self, bins: int = 50):
        assert self.fitted_ is True, 'fit() must be called before hist()'
        return plt.hist(self.correlations_, bins = bins)

import tqdm

t = train_scaled[:, :10]

to_remove = set()
threshold = 0.9 * train_scaled.shape[0]
for i in tqdm.trange(t.shape[1]):
    for j in range(i):
        if j in to_remove:
            continue
        if numpy.abs(t[:, i] @ t[:, j]) > threshold:
            to_remove.add(i)
            break

to_remove, set(range(t.shape[1])) - to_remove