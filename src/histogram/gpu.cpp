#include <gpu_examples/histogram/gpu.hpp>

GPU::GPU() {
    cl_int err;
    
    std::vector<cl::Platform> platforms;
    err = cl::Platform::get(&platforms);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to get OpenCL platforms.");
    }

    context = cl::Context(CL_DEVICE_TYPE_GPU,
                          nullptr,
                          nullptr,
                          nullptr,
                          &err);

    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create an OpenCL context.");
    }

    std::vector<cl::Device> devices = context.getInfo<CL_CONTEXT_DEVICES>();

    if(devices.size() == 0) {
        throw std::runtime_error("Did not find an OpenCL GPU device.");
    } else if(devices.size() > 1) {
        throw std::runtime_error("Expected only one OpenCL GPU device.");
    }

    dev = devices[0];

    queue = cl::CommandQueue(context, devices[0], 0 /*properties*/, &err);
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to create an OpenCL command queue.");
    }
}

GPU::~GPU() {}

void GPU::buildKernel(const std::string& name, const std::string& path) {
    cl_int err;
    std::string kernelSrc = getKernelSrc(path);

    auto program = cl::Program(context, kernelSrc.c_str(), false /*build*/, &err);
    if(err != CL_SUCCESS) {
        std::ostringstream stream;
        stream << "Failed to create an OpenCL program for the kernel " << name;
        throw std::runtime_error(stream.str().c_str());
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

    if (!kernels.emplace(name, cl::Kernel(program, name.c_str(), &err)).second) {
        std::ostringstream stream;
        stream << "Kernel name " << name << " was not unique.";
        throw std::runtime_error(stream.str().c_str());
    };

    if(err != CL_SUCCESS) {
        std::ostringstream stream;
        stream << "Failed to create the kernel " << name << ".";
        throw std::runtime_error(stream.str().c_str());
    }
}

std::string GPU::getKernelSrc(const std::string& kernelPath) {
    std::ifstream kernelFile(kernelPath);
    if(!kernelFile.is_open()) {
        throw std::runtime_error("Failed to open the kernel file.");
    }  
    kernelFile.seekg(0, std::ios::end);
    size_t fileSize = kernelFile.tellg();
    std::string kernelSrc(fileSize, ' ');
    kernelFile.seekg(0);
    kernelFile.read(&kernelSrc[0], fileSize); 
    return kernelSrc;
}

std::unique_ptr<cl::Buffer> GPU::allocateBuffer(size_t size,
                                                cl_mem_flags flags) {
    cl_int err;
    auto buf = std::make_unique<cl::Buffer>(
                      cl::Buffer(context, flags, size, nullptr, &err)
                  );
    if(err != CL_SUCCESS) {
        throw std::runtime_error("Failed to construct buffer."); 
    }

    return buf;
}
