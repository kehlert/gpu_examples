#include <chrono>

#include <gmock/gmock.h>

#include <gpu_examples/histogram/cpu_hist_generator.hpp> 
#include <gpu_examples/histogram/hist_data_reader.hpp>
#include <gpu_examples/histogram/raw_data_generator.hpp> 

TEST(CPUHistGeneratorTest, CorrectOutput) {
    std::map<int, unsigned int> expectedFrequencies;
    expectedFrequencies = HistDataReader::readFrequencies("./data/testFrequencies.txt");
    
    std::vector<int> rawData = HistDataReader::readRawData("./data/testData.txt");

    auto frequencies = CPUHistGenerator::generate(expectedFrequencies.begin()->first,
                                                  expectedFrequencies.rbegin()->first,
                                                  std::move(rawData));
    ASSERT_EQ(expectedFrequencies, frequencies);
}

TEST(CPUHistGeneratorTest, Speed) {
    using namespace std::chrono;

    const int LOWER_BOUND = -50;
    const int UPPER_BOUND = 50;
    const unsigned int N_RUNS = 1000;
    const size_t DATA_SIZE = 10000;

    //the data generation is much slower than the histogram generation
    RawDataGenerator dataGen(LOWER_BOUND, UPPER_BOUND);
    std::vector<int> data = dataGen.generate(DATA_SIZE);

    auto start = high_resolution_clock::now();
    for (unsigned int i = 0; i < N_RUNS; ++i) {
        CPUHistGenerator::generate(-50, 50, data);
    }
    auto end = high_resolution_clock::now();
    std::cout << "time: "
              << duration_cast<milliseconds>(end - start).count()
              << " ms" << std::endl;
}
