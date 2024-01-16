#include <chrono>
#include "get_input.hpp"
#include <torch/script.h>

int main(int argc, const char* argv[])
{
    std::vector<float> data = get_input();
    at::Tensor tensor_image = torch::from_blob(data.data(), { 32, 300, 300, 3 });
    tensor_image = tensor_image.permute({ 0, 3, 1, 2 });
    std::cout << "Input: " << tensor_image.sizes() << std::endl;

    if (argc < 2) throw "Model name must be specified";
    torch::jit::script::Module module;
    try { module = torch::jit::load(argv[1]); }
    catch (const c10::Error& e)
    {
        std::cerr << "error loading the model\n" << e.what();
        return -1;
    }

    int N_RUNS = 10;
    if (argc >= 3 && std::string(argv[2]) == "cuda")
    {
        tensor_image = tensor_image.to(at::kCUDA);
        module.to(at::kCUDA);
        N_RUNS = 100;
    }

    module.eval();
    torch::NoGradGuard no_grad;
    torch::jit::getProfilingMode() = false;
    torch::jit::setGraphExecutorOptimize(false);
    std::vector<c10::IValue> inputs = { tensor_image };

    c10::IValue output = module.forward(inputs);
    std::cout << output.toTensor()[0].slice(0, 0, 7).reshape({ 1, 7 }) << std::endl;
    
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N_RUNS; i++) module.forward(inputs);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << time.count() / 1e9 / N_RUNS << "s = " << time.count() / 1e6 / N_RUNS << "ms";
}