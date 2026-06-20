#include "codegen.hpp"
#include <iostream>

std::string CodeGenContext::newTemp() { return "%t" + std::to_string(tempCount++); }
std::string CodeGenContext::newLabel(const std::string& base) { return base + std::to_string(labelCount++); }

std::string CodeGenContext::escapeString(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '\\') out += "\\5C";
        else if (c == '"') out += "\\22";
        else if (c == '\n') out += "\\0A";
        else out += c;
    }
    return out;
}

std::string CodeGenContext::createStringConstant(const std::string& s) {
    std::string name = "@.str" + std::to_string(stringCount++);
    std::string escaped = escapeString(s);
    int len = (int)s.size() + 1;
    globals << name << " = private unnamed_addr constant [" << len << " x i8] c\"" << escaped << "\\00\"\n";
    return "getelementptr inbounds ([" + std::to_string(len) + " x i8], [" + std::to_string(len) + " x i8]* " + name + ", i32 0, i32 0)";
}

void CodeGenContext::emit(const std::string& line) { ir << "  " << line << "\n"; }
void CodeGenContext::emitLabel(const std::string& label) { ir << label << ":\n"; }

void CodeGenContext::declareRuntime() {
    globals << "@.fmt_int = private unnamed_addr constant [3 x i8] c\"%d\\00\"\n";
    globals << "@.fmt_space = private unnamed_addr constant [4 x i8] c\"%d \\00\"\n";
    globals << "@.fmt_nl = private unnamed_addr constant [2 x i8] c\"\\0A\\00\"\n";
    globals << "@.true = private unnamed_addr constant [6 x i8] c\"true\\0A\\00\"\n";
    globals << "@.false = private unnamed_addr constant [7 x i8] c\"false\\0A\\00\"\n";
    globals << "declare i32 @printf(i8*, ...)\n";
    globals << "declare i32 @atoi(i8*)\n\n";
}

void CodeGenContext::saveToFile(const std::string& path) {
    std::ofstream out(path);
    out << globals.str() << ir.str();
}
