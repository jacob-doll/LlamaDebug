#ifndef LLAMADEBUG_PROCESS_TYPES_H
#define LLAMADEBUG_PROCESS_TYPES_H

#include <cstdint>
#include <vector>

#ifdef _WIN32
typedef uint32_t ldpid_t;
#elif __linux__
typedef int ldpid_t;
#endif

namespace llama_debug {

struct mapped_region;
using mapped_regions_t = std::vector<mapped_region>;

}// namespace llama_debug

#endif// LLAMADEBUG_PROCESS_TYPES_H
