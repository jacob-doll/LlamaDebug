cmake_minimum_required(VERSION 3.4...3.18)
project(llama_debug)
set(CMAKE_CXX_STANDARD 17)

include(cmake/CompilerWarnings.cmake)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_subdirectory(extern/pybind11)
add_subdirectory(src)
add_subdirectory(sandbox)
# add_subdirectory(samples)

option(BUILD_LIBS "Build library" ON)

target_compile_definitions(llama_debug PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
set_project_warnings(llama_debug)
