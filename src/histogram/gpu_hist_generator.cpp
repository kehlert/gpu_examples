#include <gpu_examples/histogram/gpu_hist_generator.hpp>

GPUHistGenerator::GPUHistGenerator(const std::string& kernelPath)
: gpu(kernelPath) {}

GPUHistGenerator::GPUHistGenerator() : gpu("") {}

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lower,
                                                       const int upper,
                                                       const std::vector<int>& data) {
    if (data.size() == 0) {
        throw std::runtime_error("No data."); 
    }

    const size_t nWorkItems = 10;
    const size_t workGroupSize = 10;

    auto dataBuf = gpu.writeBuffer(data, CL_MEM_READ_ONLY);

    size_t histSize = upper - lower + 1;
    std::vector<unsigned int> freq(histSize, 0);
    auto histBuf = gpu.writeBuffer(freq, CL_MEM_WRITE_ONLY);

    size_t globalSubHistsSize = sizeof(unsigned int) * histSize * workGroupSize;
    auto globalSubHists = gpu.allocateBuffer(globalSubHistsSize,
                                             CL_MEM_WRITE_ONLY);
    gpu.runKernel(nWorkItems,
                  workGroupSize,
                  lower,
                  upper,
                  *dataBuf,
                  (unsigned int)data.size(),
                  *histBuf,
                  (unsigned int)histSize,
                  cl::Local(histSize),
                  *globalSubHists);

    freq = gpu.readBuffer<unsigned int>(*histBuf, histSize);
    std::map<int, unsigned int> hist;
    for (size_t i = 0; i < freq.size(); ++i) {
        hist[i + lower] = freq[i]; 
    }
    return hist;
}

