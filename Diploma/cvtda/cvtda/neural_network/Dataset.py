import numpy
import torch
import joblib
import torch.utils.data
import torchph.nn.slayer

import cvtda.logging


def transform(diagram: torch.Tensor, dim: int):
    dim_filter = (diagram[:, 2] == dim)
    non_degenerate_filter = (diagram[:, 0] < diagram[:, 1])
    rotation = torchph.nn.slayer.UpperDiagonalThresholdedLogTransform(0.05)
    return rotation(diagram[dim_filter & non_degenerate_filter][:, 0:2])

def process_diagram(diags: torch.Tensor):
    diagrams, non_dummy_points = [], []
    for dim in diags[:, :, 2].unique(sorted = False):
        diags_dim = [ transform(diag, dim) for diag in diags ]
        processed = torchph.nn.slayer.prepare_batch(diags_dim)
        diagrams.append(processed[0].cpu())
        non_dummy_points.append(processed[1].cpu())
    return diagrams, non_dummy_points


class Dataset(torch.utils.data.Dataset):
    def __init__(
        self,
        images: numpy.ndarray,
        diagrams: numpy.ndarray, # n_items x n_diagrams x n_points x 3
        features: numpy.ndarray,
        labels: numpy.ndarray,

        n_jobs: int = -1,
        device: torch.device = torch.device('cuda')
    ):
        self.n_jobs_ = n_jobs
        self.device_ = device

        self.images = torch.tensor(images, dtype = torch.float32)
        if len(self.images.shape) == 4:
            self.images = self.images.permute((0, 3, 1, 2))
        else:
            assert len(self.images.shape) == 3
            self.images = self.images.unsqueeze(1)
        
        self.features = torch.tensor(features, dtype = torch.float32)
        self.labels = torch.tensor(labels, dtype = torch.long)

        diagrams = [
            torch.tensor(numpy.array([ item[num_diagram] for item in diagrams ]), dtype = torch.float32)
            for num_diagram in range(len(diagrams[0]))
        ]
        diagrams = joblib.Parallel(n_jobs = self.n_jobs_)(
            joblib.delayed(process_diagram)(d)
            for d in cvtda.logging.logger().pbar(diagrams, desc = "Dataset: processing diagrams")
        )

        self.diagrams, self.non_dummy_points = [], []
        for diag, ndp in diagrams:
            self.diagrams.extend(diag)
            self.non_dummy_points.extend(ndp)
        for i in range(len(self.diagrams)):
            self.diagrams[i] = self.diagrams[i]
            self.non_dummy_points[i] = self.non_dummy_points[i]

        cvtda.logging.logger().print(
            f"Constructed a dataset of {len(self.images)} images of shape {self.images[0].shape} " +
            f"with {len(self.diagrams[0])} diagrams and {self.features.shape[1]} features"
        )

    def __len__(self):
        return len(self.images)

    def get_labels(self, idxs):
        return self.labels[idxs].to(self.device_)
    
    def get_features(self, idxs):
        output = [
            self.images[idxs].to(self.device_),
            self.features[idxs].to(self.device_) 
        ]
        for diag, ndp in zip(self.diagrams, self.non_dummy_points):
            output.append(diag[idxs].to(self.device_))
            output.append(ndp[idxs].to(self.device_))
        return output
