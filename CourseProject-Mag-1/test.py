import torch
import torchvision
import torch.utils.data
import sklearn.model_selection
import torchvision.transforms.v2

transforms = torchvision.transforms.v2.Compose(
    [
        torchvision.transforms.v2.Resize(224),
        torchvision.transforms.v2.Grayscale(num_output_channels=3),
        torchvision.transforms.v2.ToImage(),
        torchvision.transforms.v2.ToDtype(torch.float32, scale=True),
        torchvision.transforms.v2.Normalize((0.1307,), (0.3081,)),
    ]
)
train_ds = torchvision.datasets.MNIST("mnist", train=True, download=True, transform=transforms)
test_ds = torchvision.datasets.MNIST("mnist", train=False, download=True, transform=transforms)

train_idxs, _ = sklearn.model_selection.train_test_split(list(range(len(train_ds))), stratify=train_ds.targets, test_size=0.99, random_state=42)
test_idxs, _ = sklearn.model_selection.train_test_split(list(range(len(test_ds))), stratify=test_ds.targets, test_size=0.99, random_state=42)
train_y = train_ds.targets[train_idxs]
test_y = test_ds.targets[test_idxs]
train_ds = torch.utils.data.Subset(train_ds, indices=train_idxs)
test_ds = torch.utils.data.Subset(test_ds, indices=test_idxs)

len(train_idxs)

model = torchvision.models.resnet18(
    num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT
)
model.fc = torch.nn.Identity()

import zigzag.nn
hs = zigzag.nn.collect_hidden_states(model, train_ds)