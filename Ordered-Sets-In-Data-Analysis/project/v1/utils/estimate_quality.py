import typing

import numpy
import sklearn.metrics
import matplotlib.pyplot as plt

def estimate_quality(
    y_pred_proba: numpy.ndarray,
    y_true: numpy.ndarray,
    ax: typing.Optional[plt.Axes] = None,
    label_names: typing.Optional[typing.List[str]] = None,
    confusion_matrix_include_values: bool = True
) -> dict:
    if label_names is None:
        label_names = list(range(y_pred_proba.shape[1]))

    y_pred = numpy.argmax(y_pred_proba, axis = 1)
    if ax:
        sklearn.metrics.ConfusionMatrixDisplay.from_predictions(
            y_true, y_pred, ax = ax, colorbar = False,
            display_labels = label_names, include_values = confusion_matrix_include_values
        )
        ax.set_xticks(ax.get_xticks(), labels = ax.get_xticklabels(), rotation = 45, ha = "right", rotation_mode = "anchor")
        ax.set_xlabel(None)
        ax.set_ylabel(None)

    numeric_labels = list(range(y_pred_proba.shape[1]))
    auc_roc_input = y_pred_proba
    multi_class = 'ovo'
    average = 'macro'
    
    if y_pred_proba.shape[1] == 2:
        auc_roc_input = y_pred_proba[:, 1]
        multi_class = 'raise'
        average = 'binary'
    
    tn, fp, fn, tp = sklearn.metrics.confusion_matrix(y_true, y_pred).ravel()
    metrics = {
        'Accuracy':       sklearn.metrics.accuracy_score      (y_true, y_pred),
        'Precision':      sklearn.metrics.precision_score     (y_true, y_pred,        average = average,   zero_division = 0),
        'Recall':         sklearn.metrics.recall_score        (y_true, y_pred,        average = average),
        'AUC-ROC':        sklearn.metrics.roc_auc_score       (y_true, auc_roc_input, multi_class = multi_class, labels = numeric_labels),
        'F1-score':       sklearn.metrics.f1_score            (y_true, y_pred,        average = average),
        'True Positive': tp,
        'True Negative': tn,
        'False Positive': fp,
        'False Negative': fn,
        'True Negative Rate (Specificity)': tn / (tn + fp) if (tn + fp) > 0 else 0,
        'Negative Predictive Value': tn / (tn + fn) if (tn + fn) > 0 else 0,
        'False Positive Rate': fp / (fp + tn) if (fp + tn) > 0 else 0,
        'False Discovery Rate': fp / (fp + tp) if (fp + tp) > 0 else 0
    }
    for k in [ 2, 3, 5, 7, 9, 15, 30 ]:
        if y_pred_proba.shape[1] > k:
            metrics[f'TOP-{k} Accuracy'] = sklearn.metrics.top_k_accuracy_score(y_true, y_pred_proba, k = k, labels = numeric_labels)
    return metrics
