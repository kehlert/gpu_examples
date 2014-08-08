#ifndef RAW_DATA_GENERATOR_H
#define RAW_DATA_GENERATOR_H

#include <random>
#include <assert.h>

class RawDataGenerator {
public:
    RawDataGenerator(int lowerBound, int upperBound);

    virtual ~RawDataGenerator();

    virtual std::vector<int> generate(const size_t dataSize);

private:
    const size_t SEED = 0;

    std::default_random_engine engine;

    std::uniform_int_distribution<int> dist;

    RawDataGenerator();
};

#endif //RAW_DATA_GENERATOR_H
