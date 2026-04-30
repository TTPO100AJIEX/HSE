import typing

import torch
import torchvision
import cvtda.logging
import torch.utils.data
import cvtda.neural_network


def collect_hidden_states_vit(model: torchvision.models.VisionTransformer, data: torch.utils.data.DataLoader):
    x = model._process_input(data)

    batch_class_token = model.class_token.expand(x.shape[0], -1, -1)
    x = torch.cat([batch_class_token, x], dim=1)

    torch._assert(x.dim() == 3, f"Expected (batch_size, seq_length, hidden_dim) got {x.shape}")
    x = x + model.encoder.pos_embedding

    hidden_states = [x[:, -1, :]]
    for layer in model.encoder.layers:
        x = layer(x)
        hidden_states.append(x[:, -1, :])
    return hidden_states


def collect_hidden_states_batch(model: torch.nn.Module, data: torch.Tensor):
    if isinstance(model, torchvision.models.vision_transformer.VisionTransformer):
        return collect_hidden_states_vit(model, data)
    else:
        assert False, f"{type(model)} is not supported"


def collect_hidden_states(
    model: torch.nn.Module,
    data: torch.utils.data.DataLoader,
    device: torch.device = cvtda.neural_network.default_device,
) -> typing.List[torch.Tensor]:
    result = []
    model = model.to(device).eval()
    for X, *_ in cvtda.logging.logger().pbar(data, desc="Collect hidden states"):
        with torch.no_grad():
            batch_result = collect_hidden_states_batch(model, X.to(device))

        if len(result) == 0:
            # This is the first iteration, initialize result with the layer count
            result = [[batch_item.cpu()] for batch_item in batch_result]
            continue

        assert len(result) == len(batch_result), "Different number of layers for different batches?"
        for result_item, batch_item in zip(result, batch_result):
            result_item.append(batch_item.cpu())

    return [torch.concat(result_item) for result_item in result]
