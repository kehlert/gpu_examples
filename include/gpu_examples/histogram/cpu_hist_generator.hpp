#ifndef CPU_HIST_GENERATOR_H
#define CPU_HIST_GENERATOR_H

#include <vector>
#include <map>

class CPUHistGenerator {
public:
    ~CPUHistGenerator();

    static std::map<int, unsigned int> generate(const int lowerBound,
                                                      const int upperBound,
                                                      const std::vector<int>& data);

private:
    CPUHistGenerator();
};

#endif //CPU_HIST_GENERATOR_H
