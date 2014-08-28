#ifndef GPU_H
#define GPU_H

#include <stdexcept>
#include <sstream>
#include <vector>
#include <memory>
#include <fstream>
#include <regex>

#include <CL/cl.hpp>

class GPU {
public:
    GPU(const std::string& kernelPath);

    ~GPU();

    template<typename T>
    std::unique_ptr<cl::Buffer>
    writeBuffer(const std::vector<T>& data, cl_mem_flags flags) {
        const size_t SIZE = data.size() * sizeof(T);
        cl_int err;

        auto buffer = std::make_unique<cl::Buffer>(
                          cl::Buffer(context, flags, SIZE, nullptr, &err)
                      );
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to construct buffer."); 
        }

        queue.enqueueWriteBuffer(*buffer, CL_TRUE, 0, SIZE, &data[0]);
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to write buffer."); 
        }

        return buffer;
    }

    template<typename T>
    std::unique_ptr<cl::Buffer>
    writeBuffer(size_t nElements, cl_mem_flags flags) {
        const size_t SIZE = nElements * sizeof(T);
        cl_int err;

        auto buffer = std::make_unique<cl::Buffer>(
                          cl::Buffer(context, flags, SIZE, nullptr, &err)
                      );
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to construct buffer."); 
        }

        return buffer;
    }

    template<typename T>
    std::vector<T>
    readBuffer(const cl::Buffer& buffer, size_t nElements) {
        cl_int err;
        const size_t SIZE = nElements * sizeof(T);
        std::vector<T> data(nElements);

        err = queue.enqueueReadBuffer(buffer, CL_TRUE, 0, SIZE, &data[0]);
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to read buffer."); 
        }

        return data;
    }

private:
    GPU();

    static std::string getKernelSrc(const std::string& kernelPath);

    static std::string getKernelName(const std::string& src);

    cl::Context context;

    cl::CommandQueue queue;
};

#endif //GPU_H
