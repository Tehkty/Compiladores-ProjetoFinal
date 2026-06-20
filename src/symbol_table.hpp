#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP
#include <set>
#include <string>

class SymbolTable {
    std::set<std::string> vars;
public:
    bool declare(const std::string& name);
    bool exists(const std::string& name) const;
};
#endif
