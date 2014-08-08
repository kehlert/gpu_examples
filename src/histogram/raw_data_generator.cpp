#include <gpu_examples/histogram/raw_data_generator.hpp>

RawDataGenerator::RawDataGenerator()
: engine(SEED), dist(0, 0) {}

RawDataGenerator::RawDataGenerator(int lowerBound, int upperBound)
: engine(SEED), dist(lowerBound, upperBound) {
    assert(lowerBound <= upperBound);
}

RawDataGenerator::~RawDataGenerator() {}

std::vector<int> RawDataGenerator::generate(const size_t dataSize) {
    std::vector<int> data(dataSize);
    for(size_t i = 0; i < dataSize; ++i) {
        data[i] = dist(engine);
    }
    return data;
}

