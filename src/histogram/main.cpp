#include <iostream>

#include <gpu_examples/histogram/gpu_hist_generator.hpp>
#include <gpu_examples/histogram/cpu_hist_generator.hpp>
#include <gpu_examples/histogram/hist_data_reader.hpp>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: make_histogram data_file" << std::endl;
        return 1;
    }
    std::vector<int> data = HistDataReader::readRawData(argv[1]);

    GPUHistGenerator gen("./generateHistogram.cl");
    auto minMax = std::minmax_element(data.begin(), data.end());
    auto hist = gen.generate(*minMax.first, *minMax.second, data); 
    //auto hist = CPUHistGenerator::generate(*minMax.first, *minMax.second, data); 

    for(auto keyVal : hist) {
        std::cout << "bin:" << keyVal.first
                  << "\tfreq:" << keyVal.second << std::endl; 
    }

    return 0;
}
