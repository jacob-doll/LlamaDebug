#ifndef LLAMADEBUG_PARSER_H
#define LLAMADEBUG_PARSER_H

#include <cstdint>
#include <string>
#include <memory>

#include "llama_debug/binary/binary.h"
#include "llama_debug/process/process.h"

namespace llama_debug {

std::unique_ptr<binary> parse(const std::string &filename);
std::unique_ptr<binary> parse(const uint8_t *buffer, const uint32_t size);
std::unique_ptr<binary> parse(const process &proc);

}// namespace llama_debug

#endif// LLAMADEBUG_PARSER_H
