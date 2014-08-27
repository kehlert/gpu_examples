#include <gpu_examples/histogram/gpu_hist_generator.hpp>

const char* GPUHistGenerator::kernelSrc = "__kernel void doNothing() {}";

GPUHistGenerator::GPUHistGenerator() : gpu(kernelSrc) {}

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lower,
                                                       const int upper,
                                                       const std::vector<int>& data) {
    if(data.size() == 0) {
        throw std::runtime_error("No data."); 
    }

    auto dataBuf = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    auto stuff = gpu.readBuffer<int>(*dataBuf, data.size());
/*
    auto freqBuf = gpu.writeBuffer(upper - lower + 1, CL_MEM_WRITE_ONLY);
    auto frequencies = gpu.readBuffer<unsigned int>(*freqBuf, frequencies.size());

    for(const int val : frequencies) {
        std::cout << val << "," << std::flush;
    }
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequenciesMap[i + lower] = frequencies[i]; 
    }
    */
    std::map<int, unsigned int> frequenciesMap;
    return frequenciesMap;
   
}

