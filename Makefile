CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -Iheaders

SRC_MAIN = main.c
SRC_PACIENTE = src/paciente.c
SRC_AVL = src/avl.c
SRC_HEAP = src/heap.c
SRC_OP = src/op.c

OBJS = build/main.o build/paciente.o build/avl.o build/heap.o build/op.o

TARGET = programa

# ====== Build padrão ======
all: build $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# ====== Compilação dos módulos ======
build/main.o: $(SRC_MAIN) headers/paciente.h headers/avl.h headers/heap.h headers/op.h
	$(CC) $(CFLAGS) -c $(SRC_MAIN) -o $@

build/paciente.o: $(SRC_PACIENTE) headers/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_PACIENTE) -o $@

build/avl.o: $(SRC_AVL) headers/avl.h headers/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_AVL) -o $@

build/heap.o: $(SRC_HEAP) headers/heap.h headers/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_HEAP) -o $@

build/op.o: $(SRC_OP) headers/op.h headers/paciente.h headers/avl.h headers/heap.h
	$(CC) $(CFLAGS) -c $(SRC_OP) -o $@

# Criar pasta build
build:
	mkdir -p build

run: all
	./$(TARGET)
# ====== Limpeza ======
# Remove somente os .o e o binário
clean:
	rm -rf build $(TARGET)

# Remove tudo + os arquivos de dados JSON
clean-all: clean
	rm -f data/*.json
