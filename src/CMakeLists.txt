file(GLOB header_files "${CMAKE_CURRENT_SOURCE_DIR}/../include/gpu_examples/*")
file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(make_histogram ${source_files} ${header_files})
target_link_libraries(make_histogram ${OPENCL_LIB})

file(GLOB test_source_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(test_stuff ${test_source_files})
target_link_libraries(test_stuff gmock gtest gmock_main)
