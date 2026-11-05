# ==============================================================================
# Makefile para Gerenciador de Tarefas com Subtarefas (C99)
# Estrutura do projeto:
# recursividade-em-c/
#   ├── .vscode/tasks.json
#   ├── include/ (arquivos.h, menu.h, tarefas.h)
#   ├── src/     (arquivos.c, main.c, menu.c, tarefas.c)
#   ├── Makefile
#   └── README.md
# ==============================================================================

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g -Iinclude
SRC = src/main.c src/tarefas.c src/arquivos.c src/menu.c
OBJ = $(SRC:.c=.o)
TARGET = gerenciador_tarefas

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Execução com verificação de vazamento de memória (Valgrind no Linux)
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f src/*.o $(TARGET) $(TARGET).exe dados.bin agenda_*.txt relatorio_geral.txt

.PHONY: all run clean valgrind
