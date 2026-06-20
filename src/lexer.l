%option noyywrap yylineno
%{
#include "parser.hpp"
#include <cstring>
#include <cstdlib>
%}

%%
[ \t\r]+             ;
\n                   ;
\{[^}]*\}           ;
"program"            return PROGRAM;
"var"                return VAR;
"begin"              return BEGIN_T;
"end"                return END_T;
"integer"            return INTEGER;
"boolean"            return BOOLEAN;
"if"                 return IF;
"then"               return THEN;
"else"               return ELSE;
"while"              return WHILE;
"do"                 return DO;
"for"                return FOR;
"to"                 return TO;
"write"              return WRITE;
"writeln"            return WRITELN;
"argint"             return ARGINT;
"mod"                return MOD;
"true"               return TRUE_T;
"false"              return FALSE_T;
":="                 return ASSIGN;
"<="                 return LE;
">="                 return GE;
"<>"                 return NE;
[0-9]+               { yylval.number = atoi(yytext); return NUMBER; }
"\""([^"\\]|\\.)*"\"" { yylval.text = strdup(yytext + 1); yylval.text[strlen(yylval.text)-1] = 0; return STRING; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.text = strdup(yytext); return ID; }
.                    return yytext[0];
%%
