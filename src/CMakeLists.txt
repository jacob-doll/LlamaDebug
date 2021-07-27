
set(SRCS
    binary/binary.cpp
    binary/binary_pe.cpp
    binary/binary_pe64.cpp
    binary/mmap_file.cpp
)

if (WIN32)
    set(SRCS
        ${SRCS}
        debugger_win.cpp
    )

    pybind11_add_module(${PROJECT_NAME} ${SRCS} py_bindings.cpp)
    add_library(${PROJECT_NAME}_lib ${SRCS})
endif() # WIN32
