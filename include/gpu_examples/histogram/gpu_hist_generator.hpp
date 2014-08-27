#ifndef GPU_HIST_GENERATOR_H
#define GPU_HIST_GENERATOR_H

#include <vector>
#include <map>
#include <assert.h>
#include <sstream>
#include <iostream>

#include <gpu_examples/histogram/gpu.hpp>

class GPUHistGenerator {
public:
    GPUHistGenerator(const std::string& kernelPath);

    ~GPUHistGenerator();

    std::map<int, unsigned int> generate(const int lowerBound,
                                         const int upperBound,
                                         const std::vector<int>& data);

private: 
    GPUHistGenerator();

    GPU gpu;
};
#endif //GPU_HIST_GENERATOR_H
