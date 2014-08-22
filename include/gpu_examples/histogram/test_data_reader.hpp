#ifndef TEST_DATA_READER_H
#define TEST_DATA_READER_H

#include <map>
#include <vector>
#include <fstream>

class TestDataReader {
public:
    inline static std::map<int, unsigned int> readFrequencies(const std::string& filePath) {
        std::map<int, unsigned int> frequencies;
        std::ifstream freqFile(filePath);

        for (std::string line; std::getline(freqFile,line);) {
            std::istringstream lineStream(line);
            int val;
            unsigned int freq;
            lineStream >> val >> freq;
            frequencies[val] = freq;
        }

        return frequencies;
    }

    inline static std::vector<int> readRawData(const std::string& filePath) {
        std::vector<int> data;
        std::ifstream dataFile(filePath);
        for (std::string line; std::getline(dataFile, line);) {
            data.push_back(atoi(line.c_str())); 
        }
        return data;
    }

private:
    TestDataReader() {}
};


#endif //TEST_DATA_READER_H