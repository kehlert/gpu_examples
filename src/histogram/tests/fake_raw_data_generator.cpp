#include <gpu_examples/histogram/fake_raw_data_generator.hpp>

FakeRawDataGenerator::FakeRawDataGenerator(int lowerBound, int upperBound) 
    : RawDataGenerator(lowerBound, upperBound),
      lowerBound(lowerBound),
      upperBound(upperBound) {}

std::vector<int> FakeRawDataGenerator::generate(const size_t dataSize) {
    std::vector<int> data(dataSize);
    int curNum = lowerBound;
    for(auto& num : data) {
        num = curNum++;
        if(curNum > upperBound) curNum = lowerBound;
    }
    return data;
}
