
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