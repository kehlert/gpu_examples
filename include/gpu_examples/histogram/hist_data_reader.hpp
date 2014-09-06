#ifndef HIST_DATA_READER_H
#define HIST_DATA_READER_H

#include <map>
#include <vector>
#include <fstream>

class HistDataReader {
public:
    inline static std::map<int, unsigned int> readFrequencies(const std::string& filePath) {
        std::map<int, unsigned int> frequencies;
        std::ifstream freqFile(filePath);
        if(!freqFile.is_open()) {
            throw std::runtime_error("Could not open file of frequencies.");
        }

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
        if(!dataFile.is_open()) {
            throw std::runtime_error("Could not open file of frequencies.");
        }
        for (std::string line; std::getline(dataFile, line);) {
            data.push_back(atoi(line.c_str())); 
        }
        return data;
    }

private:
    HistDataReader() {}
};


#endif //HIST_DATA_READER_H
