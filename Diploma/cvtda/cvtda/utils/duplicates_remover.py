import typing

import numpy
import sklearn.base

from cvtda.logging import CLILogger, DevNullLogger
from cvtda.utils.set_random_seed import set_random_seed

class DuplicateFeaturesRemover(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,
        random_state: int = 42,

        tolerance: float = 1e-8
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.random_state_ = random_state
        set_random_seed(self.random_state_)
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.tolerance_ = tolerance
    
    def fit(self, features: numpy.ndarray):
        self.non_duplicates_ = self.find_non_duplicate_columns_(features)
        self.fitted_ = True
        return self
    
    def transform(self, features: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        return features[:, self.non_duplicates_]


    def find_duplicate_columns_(self, features: numpy.ndarray) -> set:
        duplicates = set()
        features = features.transpose()
        for i in range(features.shape[0]):
            if i in duplicates:
                continue
            comp = numpy.abs(features - features[i]) < self.tolerance_
            comp = numpy.sum(comp, axis = 1)
            comp = numpy.where(comp == features.shape[1])[0]
            for j in comp[1:]:
                duplicates.add(j)
        return duplicates

    def find_non_duplicate_columns_(self, features: numpy.ndarray) -> typing.List[int]:
        duplicates = set()

        partition_by = numpy.random.randint(low = 0, high = features.shape[0])
        partition_item = features[partition_by, :]
        for partition_value in self.logger_.loop(numpy.unique(partition_item)):
            partition_idxs = numpy.where(numpy.abs(partition_item - partition_value) < self.tolerance_)[0]
            partition_idxs = numpy.setdiff1d(partition_idxs, numpy.array(list(duplicates)), assume_unique = True)

            partition_duplicates = list(self.find_duplicate_columns_(features[:, partition_idxs]))
            for item in partition_idxs[partition_duplicates]:
                duplicates.add(item)

        self.logger_.print(f'Found {len(duplicates)} duplicates')
        return list(set(range(features.shape[1])) - duplicates)
