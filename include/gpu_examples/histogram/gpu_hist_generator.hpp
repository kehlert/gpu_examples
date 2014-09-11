#ifndef GPU_HIST_GENERATOR_H
#define GPU_HIST_GENERATOR_H

#include <vector>
#include <map>
#include <assert.h>
#include <sstream>

#include <gpu_examples/histogram/gpu.hpp>

class GPUHistGenerator {
public:
    GPUHistGenerator();

    ~GPUHistGenerator();

    std::map<int, unsigned int> generate(const int lowerBound,
                                         const int upperBound,
                                         const std::vector<int>& data);

private: 
    GPU gpu;
};
#endif //GPU_HIST_GENERATOR_H
