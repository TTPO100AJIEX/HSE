#include <chrono>
#include <vector>
#include <string>
#include <iostream>
#include "get_input.hpp"
#include <torch/script.h>

int main(int argc, const char* argv[])
{
    std::vector<float> data = get_input();
    const int batch_size = data.size() / (3 * 300 * 300);
    at::Tensor tensor_image = torch::from_blob(data.data(), { batch_size, 3, 300, 300 });
    std::cout << "Input: " << tensor_image.sizes() << std::endl;

    if (argc < 2) throw "Model name must be specified";
    torch::jit::script::Module module = torch::jit::load(argv[1]);

    int N_RUNS = 10;
    #ifdef WITH_CUDA
        if (argc >= 3 && std::string(argv[2]) == "cuda")
        {
            tensor_image = tensor_image.to(at::kCUDA);
            module.to(at::kCUDA);
            N_RUNS = 100;
        }
    #endif

    module.eval();
    torch::NoGradGuard no_grad;
    torch::jit::getProfilingMode() = false;
    torch::jit::setGraphExecutorOptimize(false);
    std::vector<c10::IValue> inputs = { tensor_image };

    c10::IValue output = module.forward(inputs);
    std::cout << output.toTensor()[0].slice(0, 0, 7).reshape({ 1, 7 }) << std::endl;
    
    #ifdef PROFILE
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < N_RUNS; i++) module.forward(inputs);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << time.count() / 1e9 / N_RUNS << "s = " << time.count() / 1e6 / N_RUNS << "ms";
    #endif
}