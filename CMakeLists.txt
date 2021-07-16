cmake_minimum_required(VERSION 3.4...3.18)
project(LlamaDebug)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(extern/pybind11)
add_subdirectory(src)
# add_subdirectory(samples)

target_compile_definitions(LlamaDebug PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

