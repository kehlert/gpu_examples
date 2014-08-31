#include<gmock/gmock.h>

#include <gpu_examples/histogram/gpu.hpp>

using ::testing::ElementsAre;

TEST(GPUTest, constructGPU) {
    GPU gpu("./doNothing.cl");
}

TEST(GPUTest, badKernelPath) {
    ASSERT_THROW(GPU gpu("badPath"), std::runtime_error);
}

TEST(GPUTest, writeBuffer) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data{1, 2, 3};
    gpu.writeBuffer(data, CL_MEM_READ_ONLY);
}

TEST(GPUTest, writeBufferZeroSize) {
    GPU gpu("./doNothing.cl");
    std::vector<int> data({});
    ASSERT_THROW(gpu.writeBuffer(data, CL_MEM_READ_ONLY), std::runtime_error);
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
    ASSERT_THROW(gpu.readBuffer<int>(*buffer, 0), std::runtime_error);
}

TEST(GPUTest, runKernel) {
    GPU gpu("./square.cl");
    std::vector<int> data{1, 2, 3};
    auto buffer = gpu.writeBuffer(data, CL_MEM_READ_WRITE);
    size_t nWorkers = 5; //more than we need
    gpu.runKernel(nWorkers, *buffer, (unsigned int)data.size());
    ASSERT_THAT(gpu.readBuffer<int>(*buffer, data.size()), ElementsAre(1, 4, 9));
}

TEST(GPUTest, noKernelArgs) {
    GPU gpu("./doNothing.cl");
    gpu.runKernel(1);
}

TEST(GPUTest, tooFewKernelArgs) {
    GPU gpu("./square.cl");
    EXPECT_THROW(gpu.runKernel(1), std::runtime_error);
}
