#include <fstream>
#include <map>
#include <chrono>

#include <gmock/gmock.h>

#include <gpu_examples/histogram/cpu_hist_generator.hpp> 
#include <gpu_examples/histogram/raw_data_generator.hpp> 

TEST(CPUHistGeneratorTest, CorrectOutput) {
    std::map<int, unsigned int> expectedFrequencies;
    std::ifstream freqFile("./data/testFrequencies.txt");

    for (std::string line; std::getline(freqFile,line);) {
        std::istringstream lineStream(line);
        int val;
        unsigned int freq;
        lineStream >> val >> freq;
        expectedFrequencies[val] = freq;
    }

    std::vector<int> data;
    std::ifstream dataFile("./data/testData.txt");
    for (std::string line; std::getline(dataFile, line);) {
        data.push_back(atoi(line.c_str())); 
    }

    CPUHistGenerator histGen(expectedFrequencies.begin()->first,
                             expectedFrequencies.rbegin()->first,
                             std::move(data));
    auto frequencies = histGen.getFrequencies();
    ASSERT_EQ(expectedFrequencies, frequencies);
}

TEST(CPUHistGeneratorTest, Speed) {
    using namespace std::chrono;

    const int LOWER_BOUND = -50;
    const int UPPER_BOUND = 50;
    const unsigned int N_RUNS = 100;
    const size_t DATA_SIZE = 10000;

    RawDataGenerator dataGen(LOWER_BOUND, UPPER_BOUND);

    auto start = high_resolution_clock::now();
    for (unsigned int i = 0; i < N_RUNS; ++i) {
        CPUHistGenerator histGen(-50, 50, dataGen.generate(DATA_SIZE));
    }
    auto end = high_resolution_clock::now();
    std::cout << "time: "
              << duration_cast<milliseconds>(end - start).count()
              << " ms" << std::endl;
    //end timer
}
