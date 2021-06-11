
set(SRCS
    main.cpp
)

if (WIN32)

set(SRCS
    ${SRCS}
    DebuggerWin.cpp
)
add_executable(LlamaDebug ${SRCS})
target_link_libraries(LlamaDebug dbgeng)

endif()
