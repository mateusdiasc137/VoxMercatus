# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -Iinclude/funcionarios -Iinclude/funcionarios/caixas
LDFLAGS = -std=c++17

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
DATA_DIR = data
TARGET = $(BIN_DIR)/supermercado.exe

# Arquivos fonte e objetos
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Regra principal
$(TARGET): $(OBJS)
	@echo "Linkando o executável: $@"
	mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Regra para compilar os objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando: $< -> $@"
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	@echo "Limpando arquivos gerados..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: clean
