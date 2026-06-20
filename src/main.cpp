#include "ast.hpp"
#include "codegen.hpp"
#include <cstdio>
#include <iostream>
#include <string>

extern FILE* yyin;
extern int yyparse();
extern Program* rootProgram;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Uso: ./minipascal arquivo.pas -o saida.ll\n";
        return 1;
    }

    std::string input = argv[1];
    std::string output = "output.ll";

    for (int i = 2; i < argc; ++i) {
        if (std::string(argv[i]) == "-o" && i + 1 < argc) {
            output = argv[i + 1];
            ++i;
        }
    }

    yyin = fopen(input.c_str(), "r");
    if (!yyin) {
        std::cerr << "Erro: nao foi possivel abrir " << input << "\n";
        return 1;
    }

    if (yyparse() != 0 || !rootProgram) {
        std::cerr << "Compilacao interrompida por erro sintatico.\n";
        fclose(yyin);
        return 1;
    }

    fclose(yyin);

    CodeGenContext ctx;
    rootProgram->codegen(ctx);
    ctx.saveToFile(output);

    std::cout << "LLVM IR gerado em: " << output << "\n";
    return 0;
}
