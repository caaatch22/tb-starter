cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(tb-starter VERSION 0.1.0)

set(CONAN_DISABLE_CHECK_COMPILER TRUE)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(tbs/include)

add_subdirectory(multi)
add_subdirectory(single)
add_subdirectory(test)
