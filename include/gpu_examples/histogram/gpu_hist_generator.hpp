#ifndef GPU_HIST_GENERATOR_H
#define GPU_HIST_GENERATOR_H

#include <vector>
#include <map>
#include <assert.h>
#include <sstream>
#include <iostream>

#include <CL/cl.hpp>

class GPUHistGenerator {
public:
    ~GPUHistGenerator();

    static std::map<int, unsigned int> generate(const int lowerBound,
                                                const int upperBound,
                                                const std::vector<int>& data);

private:
    static const char *kernel;

    GPUHistGenerator();
};

#endif //GPU_HIST_GENERATOR_H
