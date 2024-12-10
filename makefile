# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -L$(LIB_DIR) -lraylib -lm -lpthread

# Detecta sistema operacional
ifeq ($(OS),Windows_NT)
    EXEC = programa.exe
    PLATFORM = WINDOWS
    LDFLAGS += -lopengl32 -lgdi32 -lwinmm
else
    EXEC = programa
    PLATFORM = LINUX
    # Removi a flag -static para linkagem dinâmica
    # Se desejar linkagem estática, remova o comentário da linha abaixo
    # LDFLAGS += -static
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
all: setup $(TARGET)

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
	rm -f $(BIN_DIR)/*

# Atalho para rodar o programa
run: all
	./$(TARGET)

