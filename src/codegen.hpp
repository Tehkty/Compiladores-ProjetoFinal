#ifndef CODEGEN_HPP
#define CODEGEN_HPP

#include "symbol_table.hpp"
#include <fstream>
#include <map>
#include <sstream>
#include <string>

class CodeGenContext {
    int tempCount = 0;
    int labelCount = 0;
    int stringCount = 0;
public:
    std::ostringstream ir;
    std::ostringstream globals;
    SymbolTable symbols;

    std::string newTemp();
    std::string newLabel(const std::string& base);
    std::string escapeString(const std::string& s);
    std::string createStringConstant(const std::string& s);
    void emit(const std::string& line);
    void emitLabel(const std::string& label);
    void declareRuntime();
    void saveToFile(const std::string& path);
};

#endif
