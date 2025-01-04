import numpy
import torch
import pandas
import xgboost
import catboost
import sklearn.base
import sklearn.tree
import sklearn.ensemble
import sklearn.neighbors
import sklearn.preprocessing
import matplotlib.pyplot as plt

import cvtda.classification

def classify(
    train_features: numpy.ndarray,
    train_labels: numpy.ndarray,
    test_features: numpy.ndarray,
    test_labels: numpy.ndarray,

    n_jobs: int = -1,
    random_state: int = 42,

    knn_neighbours: int = 10,

    random_forest_estimators: int = 100,

    nn_device: torch.device = torch.device('cuda'),
    nn_batch_size: int = 1024,
    nn_learning_rate: float = 5e-4,
    nn_epochs: int = 50,

    grad_boost_max_iter: int = 20,
    grad_boost_max_depth: int = 4,
    grad_boost_max_features: float = 0.1,

    xgboost_n_classifiers: int = 25,
    xgboost_max_depth: int = 4,
    xgboost_device: str = 'gpu',

    catboost_iterations: int = 400,
    catboost_depth: int = 4,
    catboost_device: str = 'GPU'
):
    def classify_one(classifier: sklearn.base.ClassifierMixin, ax: plt.Axes):
        print(f'Fitting {classifier}')
        ax.set_title(type(classifier).__name__)
        classifier.fit(train_features, train_labels)
        y_pred_proba = classifier.predict_proba(test_features)
        return {
            'classifier': type(classifier).__name__,
            **cvtda.classification.estimate_quality(y_pred_proba, test_labels, ax)
        }

    classifiers = [
        sklearn.neighbors.KNeighborsClassifier(
            n_jobs = n_jobs,
            n_neighbors = knn_neighbours
        ),
        sklearn.ensemble.RandomForestClassifier(
            n_estimators = random_forest_estimators,
            random_state = random_state,
            n_jobs = n_jobs
        ),
        cvtda.classification.NNClassifier(
            random_state = random_state,
            device = nn_device,
            batch_size = nn_batch_size,
            learning_rate = nn_learning_rate,
            n_epochs = nn_epochs
        ),
        sklearn.ensemble.HistGradientBoostingClassifier(
            random_state = random_state,
            max_iter = grad_boost_max_iter,
            max_depth = grad_boost_max_depth,
            max_features = grad_boost_max_features
        ),
        xgboost.XGBClassifier(
            n_jobs = n_jobs,
            n_estimators = xgboost_n_classifiers,
            max_depth = xgboost_max_depth,
            device = xgboost_device
        ),
        catboost.CatBoostClassifier(
            iterations = catboost_iterations,
            depth = catboost_depth,
            random_seed = random_state,
            loss_function = 'MultiClass',
            devices = '0-3',
            task_type = catboost_device,
            verbose = True
        )
    ]

    figure, axes = plt.subplots(2, 3, figsize = (15, 10))
    return pandas.DataFrame([ classify_one(*args) for args in zip(classifiers, axes.flat) ])
