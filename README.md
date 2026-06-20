## Mini-Pascal Compiler
Equipe: Isabela Nunes, Pedro Huck, Sthefany Viveiros

Linguagem escolhida: Mini-Pascal

## Objetivo
Desenvolver um compilador completo para a linguagem Mini-Pascal utilizando:

* Flex (Análise Léxica)
* Bison (Análise Sintática)
* C++ (Análise Semântica)
* LLVM (Geração de Código Intermediário e Executável)

O compilador recebe um arquivo `.pas` como entrada e gera um executável nativo.


## Funcionalidades Implementadas
A linguagem Mini-Pascal desenvolvida suporta:

* Declaração de variáveis inteiras e booleanas;
* Atribuição de valores;
* Operações aritméticas e lógicas;
* Estruturas condicionais (`if` e `else`);
* Estruturas de repetição (`while` e `for`);
* Procedimentos e funções simples;
* Entrada de parâmetros pela linha de comando;
* Comandos de saída (`write` e `writeln`);
* Comentários em estilo Pascal.

## Estrutura do Projeto
```text
src/
├── lexer.l
├── parser.y
├── main.cpp
├── ast.cpp
├── codegen.cpp
└── symbol_table.cpp

tests/
├── factor.pas
├── fibonacci.pas
├── isprime.pas
└── pidigits.pas
```

## Compilação
Instalação das dependências:

```bash
sudo apt install flex bison llvm clang gcc g++ make nasm
```

Compilação do projeto:

```bash
make
```

## Execução

Exemplo de execução:

```bash
./minipascal tests/fibonacci.pas
```

## Programas de Teste
Foram utilizados os seguintes programas para validação do compilador:

### factor.pas
Realiza a fatoração prima de um número inteiro.

### isprime.pas
Verifica se um número é primo.

### fibonacci.pas
Calcula um termo da sequência de Fibonacci.

### pidigits.pas
Imprime os primeiros dígitos de π.

## Tratamento de Erros
O compilador realiza verificações léxicas, sintáticas e semânticas básicas, apresentando mensagens de erro para auxiliar na identificação de problemas no código-fonte.

## Considerações Finais
O desenvolvimento deste projeto permitiu aplicar os conceitos estudados na disciplina de Compiladores, envolvendo análise léxica, análise sintática, análise semântica e organização de um compilador utilizando Flex, Bison e C++.
