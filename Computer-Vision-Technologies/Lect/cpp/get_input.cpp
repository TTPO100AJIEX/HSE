#include <algorithm>
#include "get_input.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <PillowResize/PillowResize.hpp>

std::vector<float> get_input()
{
    cv::Mat image = cv::imread("../../../image.jpg");

    // Change BGR to RGB
    cv::cvtColor(image, image, cv::COLOR_BGR2RGB);

    // torchvision.transforms.v2.Resize(320, interpolation = torchvision.transforms.v2.InterpolationMode.BICUBIC)
    int height = image.size[0], width = image.size[1], s_min = std::min(width, height);
    cv::Size target_size = cv::Size(width * 320 / s_min, height * 320 / s_min);
    image = PillowResize::resize(image, target_size, PillowResize::INTERPOLATION_BICUBIC);

    // torchvision.transforms.v2.CenterCrop(300),
    const int cropSize = 300;
    const int offsetW = (image.cols - cropSize) / 2;
    const int offsetH = (image.rows - cropSize) / 2;
    image = image(cv::Rect(offsetW, offsetH, cropSize, cropSize));

    // torchvision.transforms.v2.ToImage(),
    // torchvision.transforms.v2.ToDtype(torch.float32, scale = True),
    cv::Mat img; image.convertTo(img, CV_32F, 1. / 255, 0);

    // torchvision.transforms.v2.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    img.forEach<cv::Vec3f>([](cv::Vec3f &pixel, __attribute__ ((unused)) const int* position) -> void
    {
        pixel[0] = (pixel[0] - 0.485) / 0.229;
        pixel[1] = (pixel[1] - 0.456) / 0.224;
        pixel[2] = (pixel[2] - 0.406) / 0.225;
    });

    img = img.reshape(1, { 300, 300, 3 });
    cv::transposeND(img, { 2, 0, 1 }, img);

    const int batch_size = 64;
    std::vector<float> result;
    img = img.reshape(1, img.total());
    result.reserve(batch_size * img.total());
    for (int i = 0; i < batch_size; i++) result.insert(result.end(), img.begin<float>(), img.end<float>());
    return result;
}