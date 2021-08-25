#ifndef LLAMADEBUG_PARSER_H
#define LLAMADEBUG_PARSER_H

#include <cstdint>
#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"

namespace llama_debug {

static std::unique_ptr<binary> parse(const std::string &filename);
static std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size);

}// namespace llama_debug

#endif// LLAMADEBUG_PARSER_H
