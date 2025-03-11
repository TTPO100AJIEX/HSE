import typing

import numpy
import gtda.diagrams

class Dataset:
    def __init__(
        self,
        images: numpy.ndarray,
        diagrams: numpy.ndarray,
        features: numpy.ndarray,
        labels: numpy.ndarray
    ):
        self.images = images
        self.diagrams = diagrams
        self.features = features
        self.labels = labels


    def process_diagrams():
        def transform(diagram, dim):
            dim_filter = (diagram[:, 2] == dim)
            non_degenerate_filter = (diagram[:, 0] != diagram[:, 1])
            rotation = torchph.nn.slayer.UpperDiagonalThresholdedLogTransform(0.05)
            return rotation(diagram[dim_filter & non_degenerate_filter][:, 0:2])

        train_data = [ ]
        test_data = [ ]
        for filtration in tqdm.tqdm(os.listdir(f"1")):
            for dim in [ 0, 1 ]:
                    dir = f"1/{filtration}"
                    train_diagrams = numpy.load(f"{dir}/train_diagrams.npy")
                    test_diagrams = numpy.load(f"{dir}/test_diagrams.npy")

                    scaler = gtda.diagrams.Scaler()
                    train_diagrams = scaler.fit_transform(train_diagrams)
                    test_diagrams = scaler.transform(test_diagrams)
                    
                    train_diagrams = torch.tensor(train_diagrams, dtype = torch.float32)
                    test_diagrams = torch.tensor(test_diagrams, dtype = torch.float32)

                    diagrams_train = joblib.Parallel(n_jobs = 1)(joblib.delayed(transform)(diagram, dim) for diagram in train_diagrams)
                    diagrams, non_dummy_points, _, _ = torchph.nn.slayer.prepare_batch(diagrams_train)
                    train_data.append(diagrams)
                    train_data.append(non_dummy_points)
                    
                    diagrams_test = joblib.Parallel(n_jobs = 1)(joblib.delayed(transform)(diagram, dim) for diagram in test_diagrams)
                    diagrams, non_dummy_points, _, _ = torchph.nn.slayer.prepare_batch(diagrams_test)
                    test_data.append(diagrams)
                    test_data.append(non_dummy_points)
        return train_data, test_data
