import os
import torch
import PIL.Image
import matplotlib.pyplot as plt
import torchvision.transforms.v2

def get_image():
    fig, axes = plt.subplots(1, 2)

    # Загрузим изображение котика
    image = PIL.Image.open(os.path.join(os.path.dirname(__file__), "image.jpg"))
    axes[0].axis('off')
    axes[0].imshow(image)

    # Будем экспериментировать с efficientnet_b3. Подготовим картинку для применения этой модели.
    # The inference transforms ... perform the following preprocessing operations:
    transform = torchvision.transforms.v2.Compose([ # Accepts PIL.Image
        # The images are resized to resize_size=[320] using interpolation=InterpolationMode.BICUBIC
        torchvision.transforms.v2.Resize(320, interpolation = torchvision.transforms.v2.InterpolationMode.BICUBIC),
        # followed by a central crop of crop_size=[300].
        torchvision.transforms.v2.CenterCrop(300),
        # Finally the values are first rescaled to [0.0, 1.0]
        torchvision.transforms.v2.ToImage(),
        torchvision.transforms.v2.ToDtype(torch.float32, scale = True),
        # and then normalized using mean=[0.485, 0.456, 0.406] and std=[0.229, 0.224, 0.225].
        torchvision.transforms.v2.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ])
    tensor = transform(image)
    axes[1].axis('off')
    axes[1].imshow(tensor.permute(1, 2, 0))

    # Создадим батч, скопировав полученную картинку 32 раза.
    input = torch.stack([ tensor ] * 32)
    print(input.shape)

    return image, tensor, input