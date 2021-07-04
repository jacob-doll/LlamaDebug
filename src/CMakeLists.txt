
set(SRCS
    main.cpp
    Binary/Binary.cpp
    Binary/BinaryPE.cpp
)

if (WIN32)

set(SRCS
    ${SRCS}
    DebuggerWin.cpp
)
add_executable(LlamaDebug ${SRCS})
target_link_libraries(LlamaDebug dbgeng)

endif()
