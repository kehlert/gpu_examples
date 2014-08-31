#include <gpu_examples/histogram/gpu_hist_generator.hpp>

GPUHistGenerator::GPUHistGenerator(const std::string& kernelPath)
: gpu(kernelPath) {}

GPUHistGenerator::GPUHistGenerator() : gpu("") {}

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lower,
                                                       const int upper,
                                                       const std::vector<int>& data) {
    if(data.size() == 0) {
        throw std::runtime_error("No data."); 
    }

    auto dataBuf = gpu.writeBuffer(data, CL_MEM_READ_ONLY);

    size_t freqSize = upper - lower + 1;
    std::vector<unsigned int> freq(freqSize, 0);
    auto freqBuf = gpu.writeBuffer(freq, CL_MEM_WRITE_ONLY);

    const size_t nWorkItems = 10;
    gpu.runKernel(nWorkItems,
                  lower,
                  upper,
                  *dataBuf,
                  (unsigned int)data.size(),
                  *freqBuf);

    freq = gpu.readBuffer<unsigned int>(*freqBuf, freqSize);

    std::map<int, unsigned int> freqMap;
    
    for (size_t i = 0; i < freq.size(); ++i) {
        freqMap[i + lower] = freq[i]; 
    }
    
    return freqMap;
}

