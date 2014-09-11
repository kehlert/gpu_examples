#include <chrono>

#include <gmock/gmock.h>

#include <gpu_examples/histogram/gpu_hist_generator.hpp> 
#include <gpu_examples/histogram/hist_data_reader.hpp>
#include <gpu_examples/histogram/raw_data_generator.hpp> 

TEST(GPUHistGeneratorTest, CorrectOutput) {
    std::map<int, unsigned int> expectedHist;
    expectedHist = HistDataReader::readFrequencies("./data/testFrequencies.txt");

    std::vector<int> rawData = HistDataReader::readRawData("./data/testData.txt");
    
    GPUHistGenerator gen;
    auto hist = gen.generate(expectedHist.begin()->first,
                             expectedHist.rbegin()->first,
                             rawData);
    ASSERT_THAT(hist, ::testing::ContainerEq(expectedHist));
}

TEST(GPUHistGeneratorTest, Speed) {
    using namespace std::chrono;

    const int LOWER_BOUND = -50;
    const int UPPER_BOUND = 50;
    const unsigned int N_RUNS = 100;
    const size_t DATA_SIZE = 10000;

    RawDataGenerator dataGen(LOWER_BOUND, UPPER_BOUND);
    GPUHistGenerator histGen;

    auto start = high_resolution_clock::now();
    for (unsigned int i = 0; i < N_RUNS; ++i) {
        histGen.generate(LOWER_BOUND, UPPER_BOUND, dataGen.generate(DATA_SIZE));
    }
    auto end = high_resolution_clock::now();
    std::cout << "time: "
              << duration_cast<milliseconds>(end - start).count()
              << " ms" << std::endl;
}
