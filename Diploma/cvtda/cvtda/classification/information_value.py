import tqdm
import numpy
import pandas
import joblib
import sklearn.base

from cvtda.logging import DevNullLogger, CLILogger


def calculate_binary_information_value(feature: numpy.ndarray, target: numpy.ndarray, bins: int = 10) -> float:
    if len(numpy.unique(feature)) > bins:
        feature = pandas.qcut(feature, bins, duplicates = 'drop')
    df = pandas.DataFrame({ 'x': feature, 'y': target })
    df = df.groupby("x", as_index = False, observed = False).agg({ "y": [ "count", "sum" ] })
    df.columns = [ 'Cutoff', 'N', 'Events' ]

    # Calculate % of events in each group.
    df['% of Events'] = numpy.maximum(df['Events'], 0.5) / df['Events'].sum()

    # Calculate the non events in each group.
    df['Non-Events'] = df['N'] - df['Events']
    # Calculate % of non events in each group.
    df['% of Non-Events'] = numpy.maximum(df['Non-Events'], 0.5) / df['Non-Events'].sum()

    # Calculate WOE by taking natural log of division of % of non-events and % of events
    df['WoE'] = numpy.log(df['% of Events'] / df['% of Non-Events'])
    df['IV'] = df['WoE'] * (df['% of Events'] - df['% of Non-Events'])
    return df['IV'].sum()

def calculate_information_value(
    features: numpy.ndarray,
    y_true: numpy.ndarray,
    bins: int = 10,
    n_jobs: int = -1,
    logger = CLILogger()
) -> pandas.DataFrame:
    def one_feature(feature_idx: int) -> dict:
        IVs = [ ]
        for class_idx in range(numpy.max(y_true)):
            target = (y_true == class_idx).astype(int)
            IVs.append(calculate_binary_information_value(features[:, feature_idx], target, bins))
        return { 'Feature': feature_idx, 'IV': numpy.mean(IVs), 'IVs': IVs }


    IV = joblib.Parallel(return_as = 'generator', n_jobs = n_jobs)(
        joblib.delayed(one_feature)(idx) for idx in range(features.shape[1])
    )
    return pandas.DataFrame(list(logger.loop(IV, total = features.shape[1], desc = 'information values')))


class InformationValueFeatureSelector(sklearn.base.TransformerMixin):
    def __init__(
        self,
        n_jobs: int = -1,
        verbose: bool = True,

        bins: int = 10,
        threshold: float = 0.5
    ):
        self.fitted_ = False
        self.n_jobs_ = n_jobs
        self.logger_ = CLILogger() if verbose else DevNullLogger()

        self.bins_ = bins
        self.threshold_ = threshold

    def fit(self, features: numpy.ndarray, target: numpy.ndarray):
        self.logger_.print('Fitting the information value feature selector')
        IV = calculate_information_value(
            features,
            target,
            bins = self.bins_,
            n_jobs = self.n_jobs_,
            logger = self.logger_
        )
        self.good_features_idx_ = IV[IV['IV'] >= self.threshold_]['Feature'].to_numpy()
        
        self.logger_.print('Fitting complete')
        self.fitted_ = True
        return self
    
    def transform(self, features: numpy.ndarray) -> numpy.ndarray:
        assert self.fitted_ is True, 'fit() must be called before transform()'
        return features[:, self.good_features_idx_]
