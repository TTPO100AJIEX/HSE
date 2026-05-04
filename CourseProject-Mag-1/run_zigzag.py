import typing

import torch
import torchvision
import torch.utils.data
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

train_y = train_ds.targets
test_y = test_ds.targets

import zigzag.nn
import zigzag.utils
import zigzag.pipelines

PARAMS = [
    zigzag.pipelines.Params(k_neighbors=2, dimension=3),
    zigzag.pipelines.Params(k_neighbors=3, dimension=3),
    zigzag.pipelines.Params(k_neighbors=4, dimension=3),
    zigzag.pipelines.Params(k_neighbors=5, dimension=3),
]

def run_model(make_model: typing.Callable[[torch.nn.Module], torch.nn.Module], model_name: str):
    dumper = zigzag.utils.UniversalDumper(f"zigzag_results/mnist/{model_name}")

    pretrained_dumper = dumper.make_subdumper("pretrained")
    model = make_model(torch.nn.Identity())
    zigzag.pipelines.validate_pretrained(model, train_ds, train_y, test_ds, test_y, pretrained_dumper)
    hidden_states = pretrained_dumper.execute(zigzag.nn.collect_hidden_states, "hidden_states", model, train_ds)
    zigzag.pipelines.analyze(hidden_states, PARAMS, pretrained_dumper, class_labels=train_y)

    finetuned_dumper = dumper.make_subdumper("finetuned")
    model = make_model(pretrained_dumper.get_dump("trained_head"))
    zigzag.pipelines.train_validate(model, train_ds, test_ds, dumper, learning_rate=1e-5)
    hidden_states = dumper.execute(zigzag.nn.collect_hidden_states, "hidden_states", model, train_ds)
    zigzag.pipelines.analyze(hidden_states, PARAMS, finetuned_dumper, class_labels=train_y)

def make_vit_b_16(head: torch.nn.Module):
    model = torchvision.models.vit_b_16(num_classes=1000, weights=torchvision.models.ViT_B_16_Weights.DEFAULT)
    model.heads = head
    return model

def make_vit_b_32(head: torch.nn.Module):
    model = torchvision.models.vit_b_32(num_classes=1000, weights=torchvision.models.ViT_B_32_Weights.DEFAULT)
    model.heads = head
    return model

def make_vit_h_14(head: torch.nn.Module):
    model = torchvision.models.vit_h_14(num_classes=1000, weights=torchvision.models.ViT_H_14_Weights.DEFAULT)
    model.heads = head
    return model

def make_vit_l_16(head: torch.nn.Module):
    model = torchvision.models.vit_l_16(num_classes=1000, weights=torchvision.models.ViT_L_16_Weights.DEFAULT)
    model.heads = head
    return model

def make_vit_l_32(head: torch.nn.Module):
    model = torchvision.models.vit_l_32(num_classes=1000, weights=torchvision.models.ViT_L_32_Weights.DEFAULT)
    model.heads = head
    return model

def make_resnet18(head: torch.nn.Module):
    model = torchvision.models.resnet18(num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT)
    model.fc = head
    return model

def make_resnet34(head: torch.nn.Module):
    model = torchvision.models.resnet34(num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT)
    model.fc = head
    return model

def make_resnet50(head: torch.nn.Module):
    model = torchvision.models.resnet50(num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT)
    model.fc = head
    return model

def make_resnet101(head: torch.nn.Module):
    model = torchvision.models.resnet101(num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT)
    model.fc = head
    return model

def make_resnet152(head: torch.nn.Module):
    model = torchvision.models.resnet152(num_classes=1000, weights=torchvision.models.ResNet18_Weights.DEFAULT)
    model.fc = head
    return model


run_model(make_vit_b_16, "vit_b_16")
run_model(make_vit_b_32, "vit_b_32")
run_model(make_vit_h_14, "vit_h_14")
run_model(make_vit_l_16, "vit_l_16")
run_model(make_vit_l_32, "vit_l_32")
run_model(make_resnet18, "resnet18")
run_model(make_resnet34, "resnet34")
run_model(make_resnet50, "resnet50")
run_model(make_resnet101, "resnet101")
run_model(make_resnet152, "resnet152")
