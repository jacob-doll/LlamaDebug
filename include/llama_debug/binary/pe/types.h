#ifndef LLAMADEBUG_PE_TYPES_H
#define LLAMADEBUG_PE_TYPES_H

#include <memory>
#include <vector>

namespace llama_debug {

class pe_export_entry;
class pe_import_entry;

using export_entry_ptr = std::shared_ptr<pe_export_entry>;
using export_entries_t = std::vector<export_entry_ptr>;

using import_entry_ptr = std::shared_ptr<pe_import_entry>;
using import_entries_t = std::vector<import_entry_ptr>;

}// namespace llama_debug


#endif// LLAMADEBUG_PE_TYPES_H