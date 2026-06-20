#include "symbol_table.hpp"

bool SymbolTable::declare(const std::string& name) {
    return vars.insert(name).second;
}

bool SymbolTable::exists(const std::string& name) const {
    return vars.count(name) > 0;
}
