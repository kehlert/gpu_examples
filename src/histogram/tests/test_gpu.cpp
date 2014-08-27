#include <gtest/gtest.h>

#include <gpu_examples/histogram/gpu.hpp>


class GPUTest : public ::testing::Test {
public:
    GPUTest() : gpu("__kernel void doNothing() {}") {}

protected:
    GPU gpu;
};

TEST_F(GPUTest, constructGPU) {
    SUCCEED();    
}

TEST_F(GPUTest, writeBuffer) {
    std::vector<int> data{1, 2, 3};
    gpu.writeBuffer(data, CL_MEM_READ_ONLY);
}

TEST_F(GPUTest, writeBufferZeroSize) {
    std::vector<int> data(0);
    EXPECT_THROW(gpu.writeBuffer(data, CL_MEM_READ_ONLY), std::runtime_error);
}

TEST_F(GPUTest, readBuffer) {
    std::vector<int> data{1, 2, 3};
    std::unique_ptr<cl::Buffer> buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    ASSERT_EQ(data, gpu.readBuffer<int>(*buffer, data.size()));
}

TEST_F(GPUTest, readBufferZeroSize) {
    std::vector<int> data{1};
    auto buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    EXPECT_THROW(gpu.readBuffer<int>(*buffer, 0), std::runtime_error);
}
