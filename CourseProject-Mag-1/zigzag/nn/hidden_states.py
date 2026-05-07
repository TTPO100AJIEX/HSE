import typing

import torch
import torchvision
import cvtda.logging
import torch.utils.data
import cvtda.neural_network


@torch.no_grad()
def yield_hidden_states_vit(
    model: torchvision.models.VisionTransformer, data: torch.Tensor
) -> typing.Generator[torch.Tensor, None, None]:
    x = model._process_input(data)

    batch_class_token = model.class_token.expand(x.shape[0], -1, -1)
    x = torch.cat([batch_class_token, x], dim=1)

    torch._assert(x.dim() == 3, f"Expected (batch_size, seq_length, hidden_dim) got {x.shape}")
    x = x + model.encoder.pos_embedding

    yield x[:, -1, :].clone()
    for layer in model.encoder.layers:
        x = layer(x)
        yield x[:, -1, :].clone()


@torch.no_grad()
def yield_hidden_states_resnet(
    model: torchvision.models.ResNet, data: torch.Tensor
) -> typing.Generator[torch.Tensor, None, None]:
    def run_block_no_relu(block: torch.nn.Module, x: torch.Tensor) -> torch.Tensor:
        if type(block) == torchvision.models.resnet.BasicBlock:
            identity = x

            out = block.conv1(x)
            out = block.bn1(out)
            out = block.relu(out)

            out = block.conv2(out)
            out = block.bn2(out)

            if block.downsample is not None:
                identity = block.downsample(x)

            out += identity
        elif type(block) == torchvision.models.resnet.Bottleneck:
            identity = x

            out = block.conv1(x)
            out = block.bn1(out)
            out = block.relu(out)

            out = block.conv2(out)
            out = block.bn2(out)
            out = block.relu(out)

            out = block.conv3(out)
            out = block.bn3(out)

            if block.downsample is not None:
                identity = block.downsample(x)

            out += identity
        else:
            assert False, f"Unknown resnet block: {type(block)}"
        return out

    hidden_states = []

    x = model.conv1(data)
    x = model.bn1(x)
    yield model.maxpool(x.clone())
    x = model.relu(x)
    x = model.maxpool(x)

    for layer in [model.layer1, model.layer2, model.layer3, model.layer4]:
        for block in layer:
            x = run_block_no_relu(block, x)
            yield x.clone()
            hidden_states.append(x.clone())
            x = block.relu(x)
    return hidden_states


@torch.no_grad()
def yield_hidden_states_batch(model: torch.nn.Module, data: torch.Tensor) -> typing.Generator[torch.Tensor, None, None]:
    if isinstance(model, torchvision.models.vision_transformer.VisionTransformer):
        return yield_hidden_states_vit(model, data)
    if isinstance(model, torchvision.models.ResNet):
        return yield_hidden_states_resnet(model, data)
    assert False, f"{type(model)} is not supported"


@torch.no_grad()
def yield_hidden_states(
    model: torch.nn.Module,
    dataset: torch.utils.data.Dataset,
    device: torch.device = cvtda.neural_network.default_device,
) -> typing.Generator[torch.Tensor, None, None]:
    model = model.to(device).eval()
    data = torch.utils.data.DataLoader(dataset, batch_size=128, shuffle=False, num_workers=3)
    data = cvtda.logging.logger().pbar(data, desc=f"Create generators")
    generators = [yield_hidden_states_batch(model, X.to(device)) for X, *_ in data]

    layer_num = 0
    while True:
        layer_num += 1
        result = []
        for batch in cvtda.logging.logger().pbar(generators, desc=f"Collect hidden states, layer {layer_num}"):
            try:
                result.append(next(batch).cpu())
            except StopIteration:
                return
        yield torch.concat(result)


@torch.no_grad()
def collect_hidden_states(
    model: torch.nn.Module,
    dataset: torch.utils.data.Dataset,
    device: torch.device = cvtda.neural_network.default_device,
) -> typing.List[torch.Tensor]:
    return list(yield_hidden_states(model, dataset, device))
