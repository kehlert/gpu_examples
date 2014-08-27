#include <chrono>

#include <gmock/gmock.h>

#include <gpu_examples/histogram/gpu_hist_generator.hpp> 
#include <gpu_examples/histogram/test_data_reader.hpp>
#include <gpu_examples/histogram/raw_data_generator.hpp> 

TEST(GPUHistGeneratorTest, DISABLED_CorrectOutput) {
    std::map<int, unsigned int> expectedFrequencies;
    expectedFrequencies = TestDataReader::readFrequencies("./data/testFrequencies.txt");

    std::vector<int> rawData = TestDataReader::readRawData("./data/testData.txt");
    
    auto gen = GPUHistGenerator("./generateHistogram.cl");
    auto frequencies = gen.generate(expectedFrequencies.begin()->first,
                                    expectedFrequencies.rbegin()->first,
                                    rawData);
    ASSERT_EQ(expectedFrequencies, frequencies);
}

TEST(GPUHistGeneratorTest, DISABLED_Speed) {
    using namespace std::chrono;

    const int LOWER_BOUND = -50;
    const int UPPER_BOUND = 50;
    const unsigned int N_RUNS = 100;
    const size_t DATA_SIZE = 10000;

    RawDataGenerator dataGen(LOWER_BOUND, UPPER_BOUND);
    auto gen = GPUHistGenerator("./generateHistogram.cl");

    auto start = high_resolution_clock::now();
    for (unsigned int i = 0; i < N_RUNS; ++i) {
        gen.generate(-50, 50, dataGen.generate(DATA_SIZE));
    }
    auto end = high_resolution_clock::now();
    std::cout << "time: "
              << duration_cast<milliseconds>(end - start).count()
              << " ms" << std::endl;
}
