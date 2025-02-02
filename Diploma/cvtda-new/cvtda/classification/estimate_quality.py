import typing

import numpy
import sklearn.metrics
import matplotlib.pyplot as plt

def estimate_quality(y_pred_proba: numpy.ndarray, y_true: numpy.ndarray, ax: typing.Optional[plt.Axes] = None) -> typing.Dict[str, float]:
    y_pred = numpy.argmax(y_pred_proba, axis = 1)
    sklearn.metrics.ConfusionMatrixDisplay.from_predictions(y_true, y_pred, ax = ax)
    return {
        'Accuracy':       sklearn.metrics.accuracy_score      (y_true, y_pred),
        'TOP-2 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 2),
        'TOP-3 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 3),
        'TOP-4 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 4),
        'TOP-5 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 5),
        'TOP-6 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 6),
        'TOP-7 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 7),
        'TOP-8 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 8),
        'TOP-9 Accuracy': sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = 9),
        'AUC-ROC':        sklearn.metrics.roc_auc_score       (y_true, y_pred_proba, multi_class = 'ovo'),
        'Precision':      sklearn.metrics.precision_score     (y_true, y_pred,       average = 'macro', zero_division = 0),
        'Recall':         sklearn.metrics.recall_score        (y_true, y_pred,       average = 'macro'),
        'F1-score':       sklearn.metrics.f1_score            (y_true, y_pred,       average = 'macro')
    }
