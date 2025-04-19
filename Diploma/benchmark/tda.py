import os
import sys
sys.path.append(os.path.abspath(os.path.join('../src')))

import numpy
import catboost

import cvtda.dumping
import cvtda.logging
import cvtda.topology

model = catboost.CatBoostClassifier(
    iterations = 400,
    depth = 4,
    random_seed = 42,
    loss_function = 'MultiClass',
    devices = '0-3',
    task_type = 'CPU',
    verbose = False
)

extractor = cvtda.topology.FeatureExtractor(n_jobs = 1)
with cvtda.logging.DevNullLogger():
    with cvtda.dumping.DevNullDumper():
        features = extractor.fit_transform(numpy.random.random((2, 100, 100)))
        model.fit(features, numpy.array([ 0, 1 ]))

model.save_model("catboost")

import time
import tqdm
import joblib

def do():
    with cvtda.logging.DevNullLogger():
        with cvtda.dumping.DevNullDumper():
            return model.predict_proba(extractor.transform(numpy.random.random((1, 100, 100))))

start = time.time()
res = [ joblib.Parallel(n_jobs = -1)(joblib.delayed(do)() for _ in tqdm.trange(200)) ]
end = time.time()

print(end - start)
