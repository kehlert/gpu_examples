FIND_LIBRARY(OPENCL_LIB OpenCL PATHS "/usr/lib")

set(OPENCL_FOUND FALSE)

IF(OPENCL_LIB)
    SET(OPENCL_FOUND TRUE)
ENDIF()

find_path(OPENCL_HEADER_DIR NAMES "cl.hpp" PATHS "$ENV{AMDAPPSDKROOT}/include/CL")

IF(OPENCL_HEADER_DIR AND OPENCL_LIB)
    SET(OPENCL_FOUND TRUE)
    SET(OPENCL_INCLUDE_DIR "$ENV{AMDAPPSDKROOT}/include/")
ELSE()
    message(FATAL "Could not find the required package OpenCL.")
ENDIF()