#include <gpu_examples/histogram/cpu_hist_generator.hpp>

CPUHistGenerator::CPUHistGenerator(int lowerBound_,
                                   int upperBound_,
                                   const std::vector<int>&& data_)
: lowerBound(lowerBound_),
  upperBound(upperBound_),
  data(data_) {}

CPUHistGenerator::~CPUHistGenerator() {}

std::map<int, unsigned int> CPUHistGenerator::getFrequencies() const {
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

CPUHistGenerator::CPUHistGenerator()
: lowerBound(0),
  upperBound(0),
  data({}) {}
