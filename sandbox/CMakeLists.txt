
add_executable(bin_test bin_test.cpp)

target_link_libraries(bin_test PUBLIC llama_debug_lib)

target_include_directories(bin_test PUBLIC 
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/include"
                          )