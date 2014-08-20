#ifndef CPU_HIST_GENERATOR_H
#define CPU_HIST_GENERATOR_H

#include <vector>
#include <map>

class CPUHistGenerator {
public:
    CPUHistGenerator(int lowerBound_,
                     int upperBound_,
                     const std::vector<int>&& data_);

    ~CPUHistGenerator();

    std::map<int, unsigned int> getFrequencies() const;

private:
    CPUHistGenerator();

    const int lowerBound;

    const int upperBound;

    const std::vector<int> data;
};

#endif //CPU_HIST_GENERATOR_H
