
set(SRCS
    py_bindings.cpp
    binary/Binary.cpp
    binary/BinaryPE.cpp
    binary/BinaryPE64.cpp
)

if (WIN32)
    set(SRCS
        ${SRCS}
        debugger_win.cpp
    )

    pybind11_add_module(${PROJECT_NAME} ${SRCS})
endif() # WIN32
