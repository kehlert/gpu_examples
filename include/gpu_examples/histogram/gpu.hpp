#ifndef GPU_H
#define GPU_H

#include <iostream>

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
    GPU();

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
    void runKernel(const std::string& kernelPath,
                   size_t nWorkItems,
                   size_t workGroupSize,
                   Args... args) {
        std::string kernelSrc = getKernelSrc(kernelPath);
        cl_int err;

        auto program = cl::Program(context, kernelSrc.c_str(), false /*build*/, &err);
        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to create an OpenCL program.");
        }

        err = program.build({dev});
        if(err != CL_SUCCESS) {
            std::string info;
            err = program.getBuildInfo<std::string>(dev,
                                                    CL_PROGRAM_BUILD_LOG,
                                                    &info);
            std::ostringstream stream;
            stream << "Failed to build the OpenCL program."
                   << std::endl << "Build log:" << std::endl;
            if(err == CL_SUCCESS) {
                stream << info;
            } else {
                stream << "Failed to get the build log.";
            }
            throw std::runtime_error(stream.str().c_str());
        }

        cl::Kernel kernel(program, getKernelName(kernelSrc).c_str(), &err);

        if(err != CL_SUCCESS) {
            throw std::runtime_error("Failed to create the kernel.");
        }

        setArgs(&kernel, 0, args...);

        err = queue.enqueueNDRangeKernel(kernel,
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
    void setArgs(cl::Kernel *kernel, size_t index) {}
#pragma GCC diagnostic pop

    template<typename T, typename... Args>
    void setArgs(cl::Kernel *kernel, size_t index, T arg, Args... args) {
        setArgs(kernel, index, arg);
        assert(sizeof...(args) > 0);
        setArgs(kernel, ++index, args...);
    }

    template<typename T>
    void setArgs(cl::Kernel *kernel, size_t index, T arg) {
        if(kernel->setArg(index, arg) != CL_SUCCESS) {
           std::string kernelName;
           kernel->getInfo(CL_KERNEL_FUNCTION_NAME, &kernelName);
          
           std::ostringstream stream;
           stream << "Failed to set kernel argument."
                  << " Kernel: " << kernelName 
                  << " Arg index: " << index;
           throw std::runtime_error(stream.str().c_str());
        }
    }

private:
    static std::string getKernelSrc(const std::string& kernelPath);

    static std::string getKernelName(const std::string& src);

    cl::Context context;

    cl::Device dev;

    cl::CommandQueue queue;
};

#endif //GPU_H
