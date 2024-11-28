# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinc
LDFLAGS = -Llib -lraylib -lm -lpthread

# Detecta sistema operacional
ifeq ($(OS),Windows_NT)
    EXEC = programa.exe
    PLATFORM = WINDOWS
    LDFLAGS += -lopengl32 -lgdi32 -lwinmm
else
    EXEC = programa
    PLATFORM = LINUX
    LDFLAGS += -ldl
endif

# Diretórios
SRC_DIR = src
INC_DIR = inc
LIB_DIR = lib
BIN_DIR = bin

# Arquivo executável
TARGET = $(BIN_DIR)/$(EXEC)

# Arquivos fonte e objetos
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

# Regras principais
all: $(TARGET)

# Gera o executável
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# Compila os arquivos .c em .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Configura diretórios necessários
setup:
	mkdir -p $(BIN_DIR)

# Limpeza dos binários e objetos
clean:
	rm -rf $(BIN_DIR)

# Atalho para rodar o programa
run: all
	./$(TARGET)
