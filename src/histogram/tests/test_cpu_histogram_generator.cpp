#include <gmock/gmock.h>
#include <gpu_examples/histogram/fake_raw_data_generator.hpp>

TEST(CPUHistogramGeneratorTest, CorrectOutput) {
    FakeRawDataGenerator rawDataGen(-5, 10);
    for(const auto num : rawDataGen.generate(5)) {
        std::cout << num << std::endl; 
    }
    //HistGenerator histGen(rawDataGen.generate());
}
