import torch
import torchph.nn.slayer
import torchvision.models


class NNBase(torch.nn.Module):
    def __init__(
        self,

        num_diagrams: int,
        skip_diagrams: bool = False,
        features_per_diagram: int = 128,

        skip_images: bool = False,
        images_output: int = 1024,

        skip_features: bool = False,
        features_output: int = 4096
    ):
        super().__init__()

        self.slayers_ = None if skip_diagrams else [ torchph.nn.slayer.SLayerExponential(features_per_diagram) for _ in range(num_diagrams) ]
        self.images_ = None if skip_images else torchvision.models.resnet18(num_classes = images_output)
        self.features_ = None if skip_features else torch.nn.LazyLinear(features_output)
    
    def forward(self, images, features, *diagrams):
        result = [ ]
        
        if self.images_ is not None:
            result.append(self.images_(images))

        if self.features_ is not None:
            result.append(self.features_(features))

        if self.slayers_ is not None:
            assert len(diagrams) == 4 * len(self.slayers_)
            for i in range(len(self.slayers_)):
                slayer_args = (diagrams[4 * i], diagrams[4 * i + 1], diagrams[4 * i + 2], diagrams[4 * i + 3])
                result.append(self.slayers_[i](slayer_args))

        return torch.cat(result, dim = 1)
