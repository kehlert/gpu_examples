#include <iostream>

#include <gpu_examples/histogram/gpu_hist_generator.hpp>
#include <gpu_examples/histogram/cpu_hist_generator.hpp>
#include <gpu_examples/histogram/hist_data_reader.hpp>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cout << "usage: make_histogram data_file n_runs" << std::endl;
        return 1;
    }


    std::vector<int> data = HistDataReader::readRawData(argv[1]);
    auto minMax = std::minmax_element(data.begin(), data.end());

    unsigned long nRuns = std::strtoul(argv[2], nullptr, 10);
    GPUHistGenerator gen;
    for(unsigned long i = 0; i < nRuns; ++i) {
        auto hist = gen.generate(*minMax.first, *minMax.second, data); 
    }
}
