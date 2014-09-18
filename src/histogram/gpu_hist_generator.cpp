#include <gpu_examples/histogram/gpu_hist_generator.hpp>

GPUHistGenerator::GPUHistGenerator() {
    gpu.buildKernel("generateSubHists", "./generateSubHists.cl");
    gpu.buildKernel("reduceSubHists", "./reduceSubHists.cl");
}

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lower,
                                                       const int upper,
                                                       const std::vector<int>& data) {
    if (data.size() == 0) {
        throw std::runtime_error("No data."); 
    }

    const size_t nWorkItems = 1024;
    const size_t workGroupSize = 128;

    auto dataBuf = gpu.writeBuffer(data, CL_MEM_READ_ONLY);

    size_t histSize = upper - lower + 1;

    size_t subHistsSize = sizeof(unsigned int) * histSize * workGroupSize;
    auto subHists = gpu.allocateBuffer(subHistsSize, CL_MEM_WRITE_ONLY);
    gpu.runKernel("generateSubHists",
                  nWorkItems,
                  workGroupSize,
                  lower,
                  upper,
                  *dataBuf,
                  (unsigned int)data.size(),
                  (unsigned int)histSize,
                  cl::Local(sizeof(unsigned int) * histSize * nWorkItems / workGroupSize),
                  *subHists);

    std::vector<unsigned int> zeros(histSize, 0);
    auto histBuf = gpu.writeBuffer(zeros, CL_MEM_WRITE_ONLY);

    gpu.runKernel("reduceSubHists",
                  histSize,
                  workGroupSize,
                  *histBuf,
                  (unsigned int)histSize,
                  *subHists,
                  (unsigned int)(nWorkItems / workGroupSize));

    auto histData = gpu.readBuffer<unsigned int>(*histBuf, histSize);
    std::map<int, unsigned int> hist;
    for (size_t i = 0; i < histData.size(); ++i) {
        hist[i + lower] = histData[i]; 
    }
    return hist;
}

