#include <gmock/gmock.h>
#include <gpu_examples/histogram/raw_data_generator.hpp>

using namespace ::testing;

MATCHER_P2(isBetween, a, b, std::string(negation ? "isn't" : "is") + " between "
            + PrintToString(a) + " and " + PrintToString(b)) {return a <= arg && arg <= b;}

TEST(RawDataGeneratorTest, CorrectDataSize) {
    RawDataGenerator gen(0, 10);
    size_t dataSize = 5;
    const std::vector<int> data = gen.generate(dataSize);
    ASSERT_EQ(dataSize, data.size());
}

TEST(RawDataGeneratorDeathTest, badLimits) {
    ASSERT_DEATH(RawDataGenerator gen(1, 0), "<");
}

TEST(RawDataGeneratorTest, WithinLimits) {
    RawDataGenerator gen(0, 1);
    size_t dataSize = 5;
    const std::vector<int> data = gen.generate(dataSize);
    for(const int num : data) {
        ASSERT_THAT(num, isBetween(0, 1)); 
    }
}
