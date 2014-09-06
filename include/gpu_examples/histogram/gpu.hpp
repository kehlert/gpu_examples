#ifndef GPU_H
#define GPU_H

#include <assert.h>

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

    std::unique_ptr<cl::Buffer> allocateBuffer(size_t size,
                                               cl_mem_flags flags = 0);

    template<typename T>
    std::unique_ptr<cl::Buffer> writeBuffer(const std::vector<T>& data,
                                            cl_mem_flags flags = 0) {
        const size_t SIZE = data.size() * sizeof(T);
        cl_int err;
        flags |= CL_MEM_COPY_HOST_PTR | CL_MEM_HOST_READ_ONLY;

        auto buf = std::make_unique<cl::Buffer>(
                          cl::Buffer(context, flags, SIZE, (void*)&data[0], &err)
                      );
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to construct buffer."); 
        }

        return buf;
    }

    template<typename T>
    std::vector<T> readBuffer(const cl::Buffer& buf, size_t nElements) {
        queue.finish();

        const size_t SIZE = nElements * sizeof(T);
        std::vector<T> data(nElements);

        cl_int err;
        err = queue.enqueueReadBuffer(buf, CL_TRUE, 0, SIZE, &data[0]);
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to read buffer."); 
        }

        return data;
    }

    template<typename... Args>
    void runKernel(size_t nWorkItems, size_t workGroupSize, Args... args) {
        setArgs(0, args...);
        cl_int err = queue.enqueueNDRangeKernel(kernel,
                                                cl::NullRange,
                                                cl::NDRange(nWorkItems),
                                                cl::NDRange(workGroupSize));
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to queue the kernel.");
        }

        err = queue.finish();
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to wait for the kernel to finish.");
        }
    }
    
#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wunused-parameter" 
    //for when the kernel doesn't take any parameters
    void setArgs(size_t index) {}
#pragma GCC diagnostic pop

    template<typename T, typename... Args>
    void setArgs(size_t index, T arg, Args... args) {
        setArgs(index, arg);
        assert(sizeof...(args) > 0);
        setArgs(++index, args...);
    }

    template<typename T>
    void setArgs(size_t index, T arg) {
        if(kernel.setArg(index, arg) != CL_SUCCESS) {
           throw std::runtime_error("Failed to set kernel argument.");
        }
    }

private:
    GPU();

    static std::string getKernelSrc(const std::string& kernelPath);

    static std::string getKernelName(const std::string& src);

    cl::Context context;

    cl::CommandQueue queue;

    cl::Kernel kernel;
};

#endif //GPU_H
