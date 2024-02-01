cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(tb-starter-multi VERSION 0.1.0)

add_subdirectory(gemm)

# file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# foreach(SRC_FILE ${SRC_FILES})
# get_filename_component(EXEC_NAME ${SRC_FILE} NAME_WE)

# add_executable(${EXEC_NAME} ${SRC_FILE})
# target_link_libraries(${EXEC_NAME} ${CONAN_LIBS})
# endforeach()
