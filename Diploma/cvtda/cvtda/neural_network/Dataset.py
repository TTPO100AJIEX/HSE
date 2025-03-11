import tqdm
import numpy
import torch
import torch.utils.data
import torchph.nn.slayer

import cvtda.logging


class Dataset(torch.utils.data.Dataset):
    def __init__(
        self,
        images: numpy.ndarray,
        diagrams: numpy.ndarray, # n_items x n_diagrams x n_points x 3
        features: numpy.ndarray,
        labels: numpy.ndarray
    ):
        self.images = torch.tensor(images)
        self.features = torch.tensor(features)
        self.labels = torch.tensor(labels)
        self.process_diagrams_(diagrams)
        cvtda.logging.logger().print(f"Constructed a dataset of {len(self.images)} images with {len(self.diagrams)} diagrams")

    def __len__(self):
        return len(self.images)

    def __getitem__(self, idx):
        output = [ self.labels[idx], self.images[idx], self.features[idx] ]
        for i in range(len(self.diagrams)):
            output.append(self.diagrams[i][idx])
            output.append(self.non_dummy_points[i][idx])
            output.append(self.batch_max_points[i][idx])
            output.append(self.batch_size[i][idx])
        return output


    def process_diagrams_(self, diagrams: numpy.ndarray):
        def transform(diagram, dim):
            dim_filter = (diagram[:, 2] == dim)
            non_degenerate_filter = (diagram[:, 0] > diagram[:, 1])
            rotation = torchph.nn.slayer.UpperDiagonalThresholdedLogTransform(0.05)
            return rotation(diagram[dim_filter & non_degenerate_filter][:, 0:2])

        self.diagrams = []
        self.non_dummy_points = []
        self.batch_max_points = []
        self.batch_size = []
        for num_diagram in tqdm.trange(len(diagrams[0]), "Dataset: processing diagrams"):
            diags = torch.tensor([ item[num_diagram] for item in diagrams ])
            for dim in diags[:, :, 2].unique(sorted = False):
                diags_dim = [ transform(diag, dim) for diag in diags ]
                processed = torchph.nn.slayer.prepare_batch(diags_dim)
                self.diagrams.append(processed[0])
                self.non_dummy_points.append(processed[1])
                self.batch_max_points.append(processed[2])
                self.batch_size.append(processed[3])
