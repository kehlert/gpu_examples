cmake_minimum_required(VERSION 2.8.12)

project(gpu_examples)

if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    message(FATAL_ERROR "Only GCC is supported.")
endif()

set(CMAKE_CXX_FLAGS "-std=c++1y -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include (CTest)

find_package(OpenCL REQUIRED)
include_directories(SYSTEM ${OPENCL_INCLUDE_DIR})

#gmock and gtest
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gmock-1.7.0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gmock-1.7.0/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gmock-1.7.0/gtest/include)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/bin")
