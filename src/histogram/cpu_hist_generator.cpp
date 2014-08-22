#include <gpu_examples/histogram/cpu_hist_generator.hpp>

CPUHistGenerator::~CPUHistGenerator() {}

std::map<int, unsigned int> CPUHistGenerator::generate(const int lowerBound,
                                                       const int upperBound,
                                                       const std::vector<int>& data) {
    std::vector<unsigned int> frequencies(upperBound - lowerBound + 1);

    for (const int val : data) {
        if (val < lowerBound || val > upperBound) {
            throw std::runtime_error("data is outside of the expected range");
        }
        ++frequencies[val - lowerBound];
    }

    std::map<int, unsigned int> frequenciesMap;
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequenciesMap[i + lowerBound] = frequencies[i]; 
    }

    return frequenciesMap;
}

CPUHistGenerator::CPUHistGenerator() {}
