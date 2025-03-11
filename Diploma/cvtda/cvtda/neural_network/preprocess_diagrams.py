



import torch

device = torch.device('cuda')

class Net(torch.nn.Module):
    def __init__(self):
        super().__init__()
        
        self.n_elements = 128
        self.n_channels = 50
        self.slayers = [
            torchph.nn.slayer.SLayerExponential(self.n_elements).to(device)
            for _ in range(self.n_channels)
        ]

        self.classifier = torch.nn.Sequential(
            torch.nn.Dropout(0.2), torch.nn.Linear(self.n_channels * self.n_elements, 1024), torch.nn.BatchNorm1d(1024), torch.nn.ReLU(),
            torch.nn.Dropout(0.1), torch.nn.Linear(1024, 512), torch.nn.BatchNorm1d(512), torch.nn.ReLU(),
            torch.nn.Linear(512, 256)
        ).to(device)
    
    def forward(self, args):
        features = [ ]
        for i in range(0, len(args), 2):
            slayer_args = (args[i].to(device), args[i + 1].to(device), args[i].shape[1], len(args[i]))
            features.append(self.slayers[i // 2](slayer_args))
        return self.classifier(torch.cat(features, dim = 1))
    
model = Net()