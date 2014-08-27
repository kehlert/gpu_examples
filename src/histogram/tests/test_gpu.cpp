#include <gtest/gtest.h>

#include <gpu_examples/histogram/gpu.hpp>

TEST(GPUTest, constructGPU) {
    GPU gpu("./doNothing.cl");
}

TEST(GPUTest, badKernelPath) {
    EXPECT_THROW(GPU gpu("badPath"), std::runtime_error);
}

TEST(GPUTest, writeBuffer) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data{1, 2, 3};
    gpu.writeBuffer(data, CL_MEM_READ_ONLY);
}

TEST(GPUTest, writeBufferZeroSize) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data(0);
    EXPECT_THROW(gpu.writeBuffer(data, CL_MEM_READ_ONLY), std::runtime_error);
}

TEST(GPUTest, writeEmptyBuffer) {
    GPU gpu("./doNothing.cl");
    gpu.writeBuffer<int>(10, CL_MEM_READ_ONLY);
}

TEST(GPUTest, writeEmptyBufferZeroSize) {
    GPU gpu("./doNothing.cl");
    EXPECT_THROW(gpu.writeBuffer<int>(0, CL_MEM_READ_ONLY), std::runtime_error);
}

TEST(GPUTest, readBuffer) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data{1, 2, 3};
    std::unique_ptr<cl::Buffer> buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    ASSERT_EQ(data, gpu.readBuffer<int>(*buffer, data.size()));
}

TEST(GPUTest, readBufferZeroSize) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data{1};
    auto buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    EXPECT_THROW(gpu.readBuffer<int>(*buffer, 0), std::runtime_error);
}

TEST(GPUTest, runKernel) {
    GPU gpu("./square.cl");
    FAIL(); //implement the rest of this test
}
