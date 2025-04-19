import torch
import torchvision

model = torchvision.models.resnet50()
model.conv1 = torch.nn.Conv2d(1, 64, kernel_size = 7, stride = 2, padding = 3, bias = False)
model.eval()

torch.save(model, "resnet50")

import time
import tqdm
import joblib

def do():
    with torch.no_grad():
        return model(torch.randn((1, 1, 100, 100))).numpy().flatten()

start = time.time()
res = [ joblib.Parallel(n_jobs = -1)(joblib.delayed(do)() for _ in tqdm.trange(200)) ]
end = time.time()
print(end - start)
