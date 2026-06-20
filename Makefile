CXX=g++
CXXFLAGS=-std=c++17 -Wall -Wextra -g
SRC=src
BUILD=build
TARGET=minipascal

all: $(TARGET)

$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/parser.cpp $(BUILD)/parser.hpp: $(SRC)/parser.y | $(BUILD)
	bison -d -o $(BUILD)/parser.cpp $(SRC)/parser.y

$(BUILD)/lexer.cpp: $(SRC)/lexer.l $(BUILD)/parser.hpp | $(BUILD)
	flex -o $(BUILD)/lexer.cpp $(SRC)/lexer.l

$(TARGET): $(BUILD)/parser.cpp $(BUILD)/lexer.cpp $(SRC)/main.cpp $(SRC)/ast.cpp $(SRC)/codegen.cpp $(SRC)/symbol_table.cpp
	$(CXX) $(CXXFLAGS) -I$(SRC) -I$(BUILD) $^ -o $(TARGET)

clean:
	rm -rf $(BUILD) $(TARGET) output/*.ll output/factor output/isprime output/fibonacci output/pidigits

.PHONY: all clean
