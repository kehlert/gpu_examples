#include <gpu_examples/histogram/gpu_hist_generator.hpp>

const char* GPUHistGenerator::kernel = "__kernel void doNothing() {}";

GPUHistGenerator::~GPUHistGenerator() {}

std::map<int, unsigned int> GPUHistGenerator::generate(const int lowerBound,
                                                       const int upperBound,
                                                       const std::vector<int>& data) {
    cl_int err;

    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to get OpenCL platforms.");
    }

    auto context = cl::Context(CL_DEVICE_TYPE_GPU,
                               nullptr,
                               nullptr,
                               nullptr,
                               &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create an OpenCL context.");
    }

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();
    if(devices.size() == 0) {
        throw std::runtime_error("Did not find an OpenCL device.");
    } else if(devices.size() > 1) {
        throw std::runtime_error("Expected only one OpenCL device.");
    }

    auto queue = cl::CommandQueue(context, devices[0], 0 /*properties*/, &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create an OpenCL command queue.");
    }
    
    auto program = cl::Program(context, kernel, false /*build*/, &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create an OpenCL program.");
    }

    err = program.build(devices);
    if(err != CL_SUCCESS) {
        std::string info;
        err = program.getBuildInfo<std::string>(devices[0],
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

    auto kernel = cl::Kernel(program, "doNothing", &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create the kernel.");
    }

    //copy this to the gpu
    std::vector<unsigned int> frequencies(upperBound - lowerBound + 1);

    //copy the data back and transform it into a map
    std::map<int, unsigned int> frequenciesMap;
    /*
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequenciesMap[i + lowerBound] = frequencies[i]; 
    }
*/
    return frequenciesMap;
}

GPUHistGenerator::GPUHistGenerator() {}
