#include <gpu_examples/histogram/raw_data_generator.hpp>

class FakeRawDataGenerator : public RawDataGenerator {
public:
    FakeRawDataGenerator(int lowerBound, int upperBound);

    virtual std::vector<int> generate(const size_t dataSize);

private:
    const int lowerBound;

    const int upperBound;
};


