cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(benchmarks VERSION 0.1.0)

# 查找所有的.cpp文件
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# 循环处理每个.cpp文件，生成对应的可执行文件
foreach(SRC_FILE ${SRC_FILES})
    # 获取.cpp文件的文件名（不含路径和扩展名）
    get_filename_component(EXEC_NAME ${SRC_FILE} NAME_WE)

    # 生成可执行文件
    add_executable(${EXEC_NAME} ${SRC_FILE})
    target_link_libraries(${EXEC_NAME} ${CONAN_LIBS})
endforeach()
