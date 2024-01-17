#include <chrono>
#include <string>
#include <vector>
#include <numeric>
#include <cstdint>
#include <iostream>
#include "get_input.hpp"
#include <onnxruntime_cxx_api.h>

int main(int argc, const char* argv[])
{
    std::vector<float> data = get_input();
    std::cout << "Input: " << data.size() << std::endl;

    Ort::Env env;
    OrtApi api = Ort::GetApi();
    Ort::SessionOptions session_options;
    Ort::AllocatorWithDefaultOptions allocator;

    int N_RUNS = 10;
    if (argc >= 3 && std::string(argv[2]) == "cuda")
    {
        OrtCUDAProviderOptionsV2* cuda_options = nullptr;
        Ort::ThrowOnError(api.CreateCUDAProviderOptions(&cuda_options));
        std::vector<const char*> keys{"device_id"};
        std::vector<const char*> values{"0"};
        Ort::ThrowOnError(api.UpdateCUDAProviderOptions(cuda_options, keys.data(), values.data(), keys.size()));
        session_options.AppendExecutionProvider_CUDA_V2(*cuda_options);
        N_RUNS = 100;
    }
    
    if (argc < 2) throw "Model name must be specified";
    Ort::Session session(env, argv[1], session_options);


    std::cout << "----------Input----------" << std::endl;
    std::cout << "Number of inputs: " << session.GetInputCount() << std::endl;
    
    Ort::AllocatedStringPtr input_name = session.GetInputNameAllocated(0, allocator);
    std::cout << "Input name: " << input_name.get() << std::endl;
    std::vector<const char*> input_names = { input_name.get() };

    Ort::TypeInfo input_type_info = session.GetInputTypeInfo(0);
    Ort::ConstTensorTypeAndShapeInfo input_tensor_info = input_type_info.GetTensorTypeAndShapeInfo();

    std::cout << "Input elements: ";
    for (int64_t dim : input_tensor_info.GetShape()) std::cout << dim << 'x';
    std::cout << "\b " << std::endl;


    std::cout << "----------Output----------" << std::endl;
    std::cout << "Number of outputs: " << session.GetOutputCount() << std::endl;
    
    Ort::AllocatedStringPtr output_name = session.GetOutputNameAllocated(0, allocator);
    std::cout << "Output name: " << output_name.get() << std::endl;
    std::vector<const char*> output_names = { output_name.get() };

    Ort::TypeInfo output_type_info = session.GetOutputTypeInfo(0);
    Ort::ConstTensorTypeAndShapeInfo output_tensor_info = output_type_info.GetTensorTypeAndShapeInfo();

    std::cout << "Output elements: ";
    for (int64_t dim : output_tensor_info.GetShape()) std::cout << dim << 'x';
    std::cout << "\b " << std::endl;


    std::vector<int64_t> shape = input_tensor_info.GetShape(); shape[0] = 32;
    Ort::MemoryInfo memory_info = Ort::MemoryInfo::CreateCpu(OrtArenaAllocator, OrtMemTypeDefault);
    Ort::Value input_tensor = Ort::Value::CreateTensor<float>(memory_info, data.data(), data.size(), shape.data(), 4);

    std::vector<Ort::Value> output_tensors = session.Run(Ort::RunOptions{}, input_names.data(), &input_tensor, 1, output_names.data(), 1);
    for (int i = 0; i < 7; i++) std::cout << output_tensors[0].At<float>({ 0, i }) << ' ';
    std::cout << std::endl;
    
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N_RUNS; i++) session.Run(Ort::RunOptions{}, input_names.data(), &input_tensor, 1, output_names.data(), 1);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << time.count() / 1e9 / N_RUNS << "s = " << time.count() / 1e6 / N_RUNS << "ms";
}