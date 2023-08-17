cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(benchmarks VERSION 0.1.0)

# 查找所有子文件夹
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# 遍历子文件夹
foreach(child ${children})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
        add_subdirectory(${child})
    endif()
endforeach()