#include<gmock/gmock.h>

#include <gpu_examples/histogram/gpu.hpp>

using ::testing::ElementsAre;

class GPUTest : public ::testing::Test {
protected:
    GPU gpu;
};

TEST_F(GPUTest, constructGPU) {}

TEST_F(GPUTest, allocateBuffer) {
    size_t size = sizeof(unsigned int) * 10;
    auto buf = gpu.allocateBuffer(size);
    ASSERT_NE(buf, nullptr);
    
    //this shouldn't throw
    gpu.readBuffer<unsigned int>(*buf, 10);
}

TEST_F(GPUTest, writeBuffer) {
    std::vector<int> data{1, 2, 3};
    gpu.writeBuffer(data, CL_MEM_READ_ONLY);
}

TEST_F(GPUTest, writeBufferZeroSize) {
    std::vector<int> data({});
    ASSERT_THROW(gpu.writeBuffer(data, CL_MEM_READ_ONLY), std::runtime_error);
}

TEST_F(GPUTest, readBuffer) {
    std::vector<int> data{1, 2, 3};
    std::unique_ptr<cl::Buffer> buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    ASSERT_EQ(data, gpu.readBuffer<int>(*buffer, data.size()));
}

TEST_F(GPUTest, readBufferZeroSize) {
    std::vector<int> data{1};
    auto buffer = gpu.writeBuffer(data, CL_MEM_READ_ONLY);
    ASSERT_THROW(gpu.readBuffer<int>(*buffer, 0), std::runtime_error);
}

TEST_F(GPUTest, buildKernel) {
    //should not throw
    gpu.buildKernel("doNothing", "./doNothing.cl");
}

TEST_F(GPUTest, nonUniqueKernelName) {
    gpu.buildKernel("doNothing", "./doNothing.cl");
    ASSERT_THROW(gpu.buildKernel("doNothing", "./doNothing.cl"),
                 std::runtime_error);
}

TEST_F(GPUTest, badKernelPath) {
    ASSERT_THROW(gpu.buildKernel("name", "badPath"), std::runtime_error);
}

TEST_F(GPUTest, wrongKernelName) {
    ASSERT_THROW(gpu.buildKernel("name", "./doNothing.cl"), std::runtime_error);
}

TEST_F(GPUTest, runKernel) {
    gpu.buildKernel("square", "./square.cl");
    std::vector<int> data{1, 2, 3};
    auto buf = gpu.writeBuffer(data, CL_MEM_READ_WRITE);
    size_t nWorkers = 5; //more than we need
    gpu.runKernel("square",
                  nWorkers,
                  nWorkers,
                  *buf,
                  (unsigned int)data.size());
    ASSERT_THAT(gpu.readBuffer<int>(*buf, data.size()), ElementsAre(1, 4, 9));
}

TEST_F(GPUTest, badKernelName) {
    ASSERT_THROW(gpu.runKernel("badName", 1, 1), std::runtime_error);
}

TEST_F(GPUTest, noKernelArgs) {
    gpu.buildKernel("doNothing", "./doNothing.cl");
    gpu.runKernel("doNothing", 1, 1);
}

TEST_F(GPUTest, tooFewKernelArgs) {
    gpu.buildKernel("square", "./square.cl");
    ASSERT_THROW(gpu.runKernel("square", 1, 1), std::runtime_error);
}
