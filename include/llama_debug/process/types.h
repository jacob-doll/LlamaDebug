#ifndef LLAMADEBUG_PROCESS_TYPES_H
#define LLAMADEBUG_PROCESS_TYPES_H

#include <cstdint>

#ifdef _WIN32
typedef uint32_t ldpid_t;
#elif __linux__
typedef int ldpid_t;
#endif

#endif// LLAMADEBUG_PROCESS_TYPES_H
