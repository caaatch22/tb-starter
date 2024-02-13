enable_language(CUDA)
enable_language(CXX)

include_directories(${CMAKE_PROGRAM_PATH}/tbs/include)

find_package(OpenMP REQUIRED)
file(GLOB HEADER_FILES *.h)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -O3 -ffast-math -fopenmp -DNEDBUG")

add_executable(gemm main.cpp ${HEADER_FILES})
target_link_libraries(gemm ${CONAN_LIBS} OpenMP::OpenMP_CXX)

add_executable(gemm_eigen gemm_eigen.cpp)
target_link_libraries(gemm_eigen ${CONAN_LIBS} OpenMP::OpenMP_CXX)

add_executable(gemm_openblas gemm_openblas.cpp)
target_link_libraries(gemm_openblas ${CONAN_LIBS} OpenMP::OpenMP_CXX)

find_package(CUDA)
if(CUDA_FOUND)
    message(STATUS "CUDA compiler: ${CMAKE_CUDA_COMPILER}")
    message(STATUS "gemm.cu path: ${CMAKE_CURRENT_SOURCE_DIR}/gemm.cu")

    # 指定CUDA标准
    set(CMAKE_CUDA_STANDARD 17)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
    set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -ccbin g++-10")

    # 添加可执行文件
    add_executable(gemm_cu gemm.cu)
    target_link_libraries(gemm_cu OpenMP::OpenMP_CXX)
    # 可选：为CUDA代码设置编译选项
    target_compile_options(gemm_cu PRIVATE $<$<COMPILE_LANGUAGE:CUDA>:-arch=sm_50 -Xcompiler=-fopenmp,-march=native>)
endif()