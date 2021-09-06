#ifndef LLAMADEBUG_BINARY_TYPES_H
#define LLAMADEBUG_BINARY_TYPES_H

#include <memory>
#include <vector>

namespace llama_debug {

class section;
class symbol;

using sections_t = std::vector<std::shared_ptr<section>>;
using symbols_t = std::vector<std::shared_ptr<symbol>>;

}// namespace llama_debug


#endif// LLAMADEBUG_BINARY_TYPES_H
