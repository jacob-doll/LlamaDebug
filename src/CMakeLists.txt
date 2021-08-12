
set(SRCS
    binary/binary.cpp
    binary/mmap_file.cpp
    binary/section.cpp
    binary/pe/binary_pe.cpp
    binary/pe/defs.cpp
    binary/pe/dos_header.cpp
    binary/pe/file_header.cpp
    binary/pe/import_directory.cpp
    binary/pe/optional_header.cpp
    binary/pe/section_header.cpp
)

if (WIN32)
    set(SRCS
        ${SRCS}
        debugger_win.cpp
    )

    pybind11_add_module(${PROJECT_NAME} ${SRCS} py_bindings.cpp)
    add_library(${PROJECT_NAME}_lib ${SRCS})
endif() # WIN32
