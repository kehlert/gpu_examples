file(GLOB source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM source_files ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_library(make_histogram_lib ${source_files})
target_link_libraries(make_histogram_lib ${OPENCL_LIB})

#main executable
add_executable(make_histogram "./main.cpp")
target_link_libraries(make_histogram make_histogram_lib)

#test executable
file(GLOB test_source_files "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp")
add_executable(test_make_histogram ${test_source_files})
target_link_libraries(test_make_histogram make_histogram_lib gmock gtest gmock_main)
add_test(test_make_histogram test_make_histogram)

add_custom_command(TARGET test_make_histogram
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/testFrequencies.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/data/testFrequencies.txt")

add_custom_command(TARGET test_make_histogram
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/data/testData.txt"
    "${CMAKE_CURRENT_BINARY_DIR}/data/testData.txt")
