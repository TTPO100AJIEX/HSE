import pandas
import torch
import torchvision
import matplotlib.pyplot as plt
import torchvision.transforms.v2

import zigzag.utils
from zigzag.pipelines.validate import train_validate, validate_pretrained

def hidden_state_layers(model: torch.nn.Module) -> list[tuple[str, tuple[int, ...]]]:
    if isinstance(model, torchvision.models.VisionTransformer):
        return [(f'encoder.layers.{i}', (i,)) for i, _ in enumerate(model.encoder.layers)]
    if isinstance(model, torchvision.models.ResNet):
        return [
            (f'layer{stage}.{block}', (stage, block))
            for stage in range(1, 5)
            for block, _ in enumerate(getattr(model, f'layer{stage}'))
        ]
    raise TypeError(f'Unsupported architecture: {type(model)}')

class ResidualBlockBypass(torch.nn.Module):
    def __init__(self, block: torch.nn.Module):
        super().__init__()
        self.downsample = block.downsample

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        return x if self.downsample is None else self.downsample(x)

def remove_hidden_state_layer(model: torch.nn.Module, location: tuple[int, ...]) -> None:
    if isinstance(model, torchvision.models.VisionTransformer):
        model.encoder.layers[location[0]] = torch.nn.Identity()
    elif isinstance(model, torchvision.models.ResNet):
        blocks = getattr(model, f'layer{location[0]}')
        blocks[location[1]] = ResidualBlockBypass(blocks[location[1]])
    else:
        raise TypeError(f'Unsupported architecture: {type(model)}')
        
def final_accuracy(history: pandas.DataFrame) -> float:
    for column in ('Accuracy', 'accuracy'):
        if column in history:
            return float(history[column].iloc[-1])
    raise KeyError(f'No accuracy column in training history: {list(history.columns)}')

def run_ablations(model_name, make_model) -> pandas.DataFrame:
    _, transforms = make_model(torch.nn.Identity())
    transforms = torchvision.transforms.v2.Compose([torchvision.transforms.v2.Grayscale(num_output_channels=3), transforms()])
    train_ds = torchvision.datasets.MNIST("mnist", train=True, download=True, transform=transforms)
    test_ds = torchvision.datasets.MNIST("mnist", train=False, download=True, transform=transforms)
    train_y = train_ds.targets
    test_y = test_ds.targets

    dumper = zigzag.utils.UniversalDumper(f'ablation_results/mnist/{model_name}')

    model, _ = make_model(torch.nn.Identity())
    pretrained_dumper = dumper.make_subdumper('pretrained')
    validate_pretrained(model, train_ds, train_y, test_ds, test_y, pretrained_dumper)

    finetuned_dumper = dumper.make_subdumper('finetuned')
    model, _ = make_model(pretrained_dumper.get_dump('trained_head'))
    train_validate(model, train_ds, test_ds, finetuned_dumper, learning_rate=1e-5)
    baseline_accuracy = final_accuracy(finetuned_dumper.get_dump('train_model_history'))
    print('Baseline accuracy:', baseline_accuracy)

    results = []
    for index, (layer_name, location) in enumerate(hidden_state_layers(make_model(torch.nn.Identity())[0]), start=1):
        print(f"Trying layer {layer_name}, {location}")
        layer_dumper = dumper.make_subdumper(f'{index:02d}_{layer_name}')
        ablated_model = finetuned_dumper.get_dump("trained_model")
        remove_hidden_state_layer(ablated_model, location)
        train_validate(ablated_model, train_ds, test_ds, layer_dumper, learning_rate=1e-5)
        accuracy = final_accuracy(layer_dumper.get_dump('train_model_history'))
        results.append({
            'model': model_name,
            'layer_index': index,
            'layer': layer_name,
            'baseline_accuracy': baseline_accuracy,
            'ablated_accuracy': accuracy,
            'accuracy_degradation': baseline_accuracy - accuracy
        })
    results = pandas.DataFrame(results)
    dumper.save_dump(results, 'summary')
    return results

def make_vit_b_32(head: torch.nn.Module):
    model = torchvision.models.vit_b_32(num_classes=1000, weights=torchvision.models.ViT_B_32_Weights.DEFAULT)
    model.heads = head
    return model, torchvision.models.ViT_B_32_Weights.DEFAULT.transforms

def make_vit_l_16(head: torch.nn.Module):
    model = torchvision.models.vit_l_16(num_classes=1000, weights=torchvision.models.ViT_L_16_Weights.DEFAULT)
    model.heads = head
    return model, torchvision.models.ViT_L_16_Weights.DEFAULT.transforms

def make_resnet34(head: torch.nn.Module):
    model = torchvision.models.resnet34(num_classes=1000, weights=torchvision.models.ResNet34_Weights.DEFAULT)
    model.fc = head
    return model, torchvision.models.ResNet34_Weights.DEFAULT.transforms

run_ablations("vit_b_32", make_vit_b_32)
run_ablations("vit_l_16", make_vit_l_16)
run_ablations("resnet34", make_resnet34)
