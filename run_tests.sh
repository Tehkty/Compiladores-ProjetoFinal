#!/bin/bash
set -e

mkdir -p output
make

for name in factor isprime fibonacci pidigits; do
  echo "Compilando tests/$name.pas..."
  ./minipascal tests/$name.pas -o output/$name.ll
  clang output/$name.ll -o output/$name
  echo "OK: output/$name"
done

echo ""
echo "Executando testes:"
echo "factor 84 => esperado: 2 2 3 7"
./output/factor 84

echo "isprime 7 => esperado: true"
./output/isprime 7

echo "fibonacci 10 => esperado: 55"
./output/fibonacci 10

echo "pidigits 5 => esperado: 31415"
./output/pidigits 5
