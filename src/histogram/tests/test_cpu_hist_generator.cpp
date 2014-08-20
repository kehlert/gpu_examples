#include <fstream>
#include <map>
#include <gmock/gmock.h>
#include <gpu_examples/histogram/cpu_hist_generator.hpp> 

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
