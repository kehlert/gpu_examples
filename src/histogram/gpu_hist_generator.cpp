#include <gpu_examples/histogram/gpu_hist_generator.hpp>

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lowerBound,
                                                       const int upperBound,
                                                       const std::vector<int>& data) {
    //copy this to the gpu
    std::vector<unsigned int> frequencies(upperBound - lowerBound + 1);

    //copy the data back and transform it into a map
    std::map<int, unsigned int> frequenciesMap;
    /*
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequenciesMap[i + lowerBound] = frequencies[i]; 
    }
*/
    return frequenciesMap;
}

GPUHistGenerator::GPUHistGenerator() {}
