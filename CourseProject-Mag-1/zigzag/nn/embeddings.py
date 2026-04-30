import torch
import cvtda.logging
import torch.utils.data
import cvtda.neural_network


def precompute_embeddings(
    model: torch.nn.Module,
    train_dl: torch.utils.data.DataLoader,
    device: torch.device = cvtda.neural_network.default_device,
) -> torch.Tensor:
    model = model.to(device).eval()
    with torch.no_grad():
        embeddings = []
        for X, _ in cvtda.logging.logger().pbar(train_dl, desc="Precompute emeddings"):
            embeddings.append(model(X.to(device)))
    return torch.concat(embeddings).cpu()
