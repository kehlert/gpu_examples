#include <gtest/gtest.h>
#include <gpu_examples/histogram/raw_data_generator.hpp>

TEST(RawDataGeneratorTest, CorrectDataSize) {
    RawDataGenerator gen(0, 10);
    size_t dataSize = 5;
    const std::vector<int> data = gen.generate(dataSize);
    EXPECT_EQ(dataSize, data.size());
}

TEST(RawDataGeneratorDeathTest, badLimits) {
    EXPECT_DEATH(RawDataGenerator gen(1, 0), "<");
}

//test that lowerLimit is < upperLimit

//test the upper and lower data limits (make sure data is at least in between)

//test that data contains at least two different values
