
set(SRCS
    binary/binary.cpp
    binary/mmap_file.cpp
    binary/parser.cpp
    binary/section.cpp
    binary/symbol.cpp
    binary/pe/defs.cpp
    binary/pe/pe_binary.cpp
    binary/pe/pe_dos_header.cpp
    binary/pe/pe_export_directory.cpp
    binary/pe/pe_export_entry.cpp
    binary/pe/pe_file_header.cpp
    binary/pe/pe_import_directory.cpp
    binary/pe/pe_import_entry.cpp
    binary/pe/pe_optional_header.cpp
    binary/pe/pe_parser.cpp
    binary/pe/pe_resource_data_entry.cpp
    binary/pe/pe_resource_directory_entry.cpp
    binary/pe/pe_resource_directory.cpp
    binary/pe/pe_section_header.cpp
)

if (WIN32)
    set(SRCS
        ${SRCS}
        debugger_win.cpp
    )

    pybind11_add_module(${PROJECT_NAME} ${SRCS} py_bindings.cpp)
    add_library(${PROJECT_NAME}_lib ${SRCS})
endif() # WIN32
