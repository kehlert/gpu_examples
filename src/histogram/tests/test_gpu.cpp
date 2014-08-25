#include <gtest/gtest.h>

#include <gpu_examples/histogram/gpu.hpp>

const char* kernelSrc = "__kernel void doNothing() {}";

TEST(GPUTest, constructDevice) {
    GPU gpu(kernelSrc);
}

TEST(GPUTest, writeBuffer) {
    GPU gpu(kernelSrc);
    std::vector<int> data{1, 2, 3};
    gpu.writeBuffer(data, CL_MEM_READ_ONLY);
}

TEST(GPUTest, writeBufferZeroSize) {
    GPU gpu(kernelSrc);
    std::vector<int> data(0);
    EXPECT_THROW(gpu.writeBuffer(data, CL_MEM_READ_ONLY), std::runtime_error);
}

TEST(GPUTest, readBuffer) {
    GPU gpu(kernelSrc);
    std::vector<int> data{1, 2, 3};
    std::unique_ptr<cl::Buffer> buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    ASSERT_EQ(data, gpu.readBuffer<int>(*buffer, data.size()));
}

TEST(GPUTest, readBufferZeroSize) {
    GPU gpu(kernelSrc);
    std::vector<int> data{1};
    auto buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    EXPECT_THROW(gpu.readBuffer<int>(*buffer, 0), std::runtime_error);
}
