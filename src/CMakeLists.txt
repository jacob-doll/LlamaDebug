
set(SRCS
    Bindings.cpp
    Binary/Binary.cpp
    Binary/BinaryPE.cpp
    Binary/BinaryPE64.cpp
)

if (WIN32)
    set(SRCS
        ${SRCS}
        DebuggerWin.cpp
    )

    pybind11_add_module(${PROJECT_NAME} ${SRCS})
endif() # WIN32
