#include <gpu_examples/histogram/gpu_hist_generator.hpp>

const char* GPUHistGenerator::kernelSrc = "__kernel void doNothing() {}";

GPUHistGenerator::GPUHistGenerator() : gpu(kernelSrc) {}

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lowerBound,
                                                       const int upperBound,
                                                       const std::vector<int>& data) {
    if(data.size() == 0) {
        throw std::runtime_error("No data."); 
    }

    const size_t DATA_SIZE = data.size() * sizeof(data[0]);

    cl_int err;

    auto dataBuffer = cl::Buffer(gpu.context,
                                 CL_MEM_READ_ONLY,
                                 DATA_SIZE,
                                 nullptr,
                                 &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to construct data buffer."); 
    }

    auto freqBuffer = cl::Buffer(gpu.context,
                                 CL_MEM_WRITE_ONLY,
                                 DATA_SIZE,
                                 nullptr,
                                 &err);
    if(err != CL_SUCCESS) { 
        throw std::runtime_error("Failed to construct results buffer."); 
    }

    err = gpu.queue.enqueueWriteBuffer(dataBuffer, CL_TRUE, 0, DATA_SIZE, &data[0]);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to write an OpenCL buffer."); 
    }

    std::vector<int> frequencies(upperBound - lowerBound + 1);
    /*
    err = queue.enqueueReadBuffer(freqBuffer, CL_TRUE, 0, DATA_SIZE, &frequencies[0]);
    for(const int val : frequencies) {
        std::cout << val << "," << std::flush;
    }
   */ 
    std::map<int, unsigned int> frequenciesMap;
    /*
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequenciesMap[i + lowerBound] = frequencies[i]; 
    }
  */ 
    return frequenciesMap;
}

