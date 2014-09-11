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

std::string GPU::getKernelName(const std::string& src) {
    static const std::regex nameRegex("kernel void ([[:alpha:]]+)\\(");
    std::smatch match;
    if(!std::regex_search(src, match, nameRegex)) {
        throw std::runtime_error("Couldn't find the kernel name in the .cl file.");
    }
    return match[1];
}

std::unique_ptr<cl::Buffer> GPU::allocateBuffer(size_t size,
                                                cl_mem_flags flags) {
    cl_int err;
    auto buf = std::make_unique<cl::Buffer>(
                      cl::Buffer(context, flags, size, nullptr, &err)
                  );
    if(err != CL_SUCCESS) {
        std::cout << "err:" << err << std::endl;
        throw std::runtime_error("Failed to construct buffer."); 
    }

    return buf;
}
