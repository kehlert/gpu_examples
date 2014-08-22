#ifndef GPU_HIST_GENERATOR_H
#define GPU_HIST_GENERATOR_H

#include <vector>
#include <map>

class GPUHistGenerator {
public:
    ~GPUHistGenerator();

    static std::map<int, unsigned int> generate(const int lowerBound,
                                                const int upperBound,
                                                const std::vector<int>& data);

private:
    GPUHistGenerator();
};

#endif //GPU_HIST_GENERATOR_H
