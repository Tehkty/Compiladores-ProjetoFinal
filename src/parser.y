%code requires {
#include "ast.hpp"
#include <string>
#include <vector>
}

%{
#include "ast.hpp"
#include <cstdio>
#include <cstdlib>
#include <iostream>

extern int yylex();
extern int yylineno;
extern FILE* yyin;
void yyerror(const char* s);
Program* rootProgram = nullptr;
%}

%union {
    int number;
    char* text;
    Expr* expr;
    Stmt* stmt;
    std::vector<Expr*>* exprList;
    std::vector<Stmt*>* stmtList;
    std::vector<std::string>* strList;
    Program* program;
}

%token PROGRAM VAR BEGIN_T END_T INTEGER BOOLEAN IF THEN ELSE WHILE DO FOR TO WRITE WRITELN ARGINT MOD TRUE_T FALSE_T
%token ASSIGN LE GE NE
%token <number> NUMBER
%token <text> ID STRING

%type <program> program
%type <strList> var_section decl_list id_list
%type <stmtList> stmt_list block
%type <stmt> stmt assignment write_stmt if_stmt while_stmt for_stmt
%type <expr> expr term factor rel_expr
%type <exprList> expr_list

%left '=' NE '<' '>' LE GE
%left '+' '-'
%left '*' '/' MOD

%%

program:
    PROGRAM ID ';' var_section block '.' { rootProgram = new Program($2, $4, $5); free($2); $$ = rootProgram; }
;

var_section:
    VAR decl_list { $$ = $2; }
  | /* empty */ { $$ = new std::vector<std::string>(); }
;

decl_list:
    decl_list id_list ':' INTEGER ';' { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end()); delete $2; }
  | decl_list id_list ':' BOOLEAN ';' { $$ = $1; $$->insert($$->end(), $2->begin(), $2->end()); delete $2; }
  | id_list ':' INTEGER ';' { $$ = $1; }
  | id_list ':' BOOLEAN ';' { $$ = $1; }
;

id_list:
    ID { $$ = new std::vector<std::string>(); $$->push_back($1); free($1); }
  | id_list ',' ID { $$ = $1; $$->push_back($3); free($3); }
;

block:
    BEGIN_T stmt_list END_T { $$ = $2; }
;

stmt_list:
    stmt_list stmt ';' { $$ = $1; $$->push_back($2); }
  | stmt ';' { $$ = new std::vector<Stmt*>(); $$->push_back($1); }
  | /* empty */ { $$ = new std::vector<Stmt*>(); }
;

stmt:
    assignment { $$ = $1; }
  | write_stmt { $$ = $1; }
  | if_stmt { $$ = $1; }
  | while_stmt { $$ = $1; }
  | for_stmt { $$ = $1; }
  | block { $$ = new BlockStmt($1); }
;

assignment:
    ID ASSIGN expr { $$ = new AssignStmt($1, $3); free($1); }
;

write_stmt:
    WRITE '(' expr_list ')' { $$ = new WriteStmt(false, $3); }
  | WRITELN '(' expr_list ')' { $$ = new WriteStmt(true, $3); }
  | WRITELN '(' ')' { $$ = new WriteStmt(true, new std::vector<Expr*>()); }
;

expr_list:
    expr { $$ = new std::vector<Expr*>(); $$->push_back($1); }
  | expr_list ',' expr { $$ = $1; $$->push_back($3); }
;

if_stmt:
    IF rel_expr THEN stmt ELSE stmt { auto* t = new std::vector<Stmt*>(); t->push_back($4); auto* e = new std::vector<Stmt*>(); e->push_back($6); $$ = new IfStmt($2, t, e); }
  | IF rel_expr THEN stmt { auto* t = new std::vector<Stmt*>(); t->push_back($4); $$ = new IfStmt($2, t, new std::vector<Stmt*>()); }
;

while_stmt:
    WHILE rel_expr DO stmt { auto* b = new std::vector<Stmt*>(); b->push_back($4); $$ = new WhileStmt($2, b); }
;

for_stmt:
    FOR ID ASSIGN expr TO expr DO stmt { auto* b = new std::vector<Stmt*>(); b->push_back($8); $$ = new ForStmt($2, $4, $6, b); free($2); }
;

rel_expr:
    expr { $$ = $1; }
  | expr '=' expr { $$ = new BinaryExpr("=", $1, $3); }
  | expr NE expr { $$ = new BinaryExpr("<>", $1, $3); }
  | expr '<' expr { $$ = new BinaryExpr("<", $1, $3); }
  | expr '>' expr { $$ = new BinaryExpr(">", $1, $3); }
  | expr LE expr { $$ = new BinaryExpr("<=", $1, $3); }
  | expr GE expr { $$ = new BinaryExpr(">=", $1, $3); }
;

expr:
    expr '+' term { $$ = new BinaryExpr("+", $1, $3); }
  | expr '-' term { $$ = new BinaryExpr("-", $1, $3); }
  | term { $$ = $1; }
;

term:
    term '*' factor { $$ = new BinaryExpr("*", $1, $3); }
  | term '/' factor { $$ = new BinaryExpr("/", $1, $3); }
  | term MOD factor { $$ = new BinaryExpr("mod", $1, $3); }
  | factor { $$ = $1; }
;

factor:
    NUMBER { $$ = new NumberExpr($1); }
  | TRUE_T { $$ = new NumberExpr(1); }
  | FALSE_T { $$ = new NumberExpr(0); }
  | ID { $$ = new VariableExpr($1); free($1); }
  | STRING { $$ = new StringExpr($1); free($1); }
  | ARGINT '(' expr ')' { $$ = new ArgIntExpr($3); }
  | '(' expr ')' { $$ = $2; }
;

%%

void yyerror(const char* s) {
    std::cerr << "Erro sintatico na linha " << yylineno << ": " << s << std::endl;
}
