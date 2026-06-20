# Gramática Mini-Pascal

Resumo das principais regras livres de contexto usadas no `parser.y`.

```txt
program:
    PROGRAM ID ';' var_section block '.'

var_section:
    VAR decl_list
  | vazio

decl_list:
    decl_list id_list ':' INTEGER ';'
  | decl_list id_list ':' BOOLEAN ';'
  | id_list ':' INTEGER ';'
  | id_list ':' BOOLEAN ';'

block:
    BEGIN_T stmt_list END_T

stmt:
    assignment
  | write_stmt
  | if_stmt
  | while_stmt
  | for_stmt
  | block

assignment:
    ID ASSIGN expr

write_stmt:
    WRITE '(' expr_list ')'
  | WRITELN '(' expr_list ')'
  | WRITELN '(' ')'

if_stmt:
    IF rel_expr THEN stmt ELSE stmt
  | IF rel_expr THEN stmt

while_stmt:
    WHILE rel_expr DO stmt

for_stmt:
    FOR ID ASSIGN expr TO expr DO stmt

expr:
    expr '+' term
  | expr '-' term
  | term

term:
    term '*' factor
  | term '/' factor
  | term MOD factor
  | factor

factor:
    NUMBER
  | TRUE_T
  | FALSE_T
  | ID
  | STRING
  | ARGINT '(' expr ')'
  | '(' expr ')'
```
