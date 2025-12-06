CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRC_MAIN = main.c
SRC_PACIENTE = TADS/PACIENTE/paciente.c
SRC_AVL = TADS/AVL/avl.c
SRC_HEAP = TADS/HEAP/heap.c
SRC_OP = TADS/OP/op.c

OBJS = build/main.o build/paciente.o build/avl.o build/heap.o build/op.o

TARGET = programa

# ====== Build padrão ======
all: build $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# ====== Compilação dos módulos ======
build/main.o: $(SRC_MAIN)
	$(CC) $(CFLAGS) -c $(SRC_MAIN) -o $@

build/paciente.o: $(SRC_PACIENTE) TADS/PACIENTE/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_PACIENTE) -o $@

build/avl.o: $(SRC_AVL) TADS/AVL/avl.h
	$(CC) $(CFLAGS) -c $(SRC_AVL) -o $@

build/heap.o: $(SRC_HEAP) TADS/HEAP/heap.h
	$(CC) $(CFLAGS) -c $(SRC_HEAP) -o $@

build/op.o: $(SRC_OP) TADS/OP/op.h
	$(CC) $(CFLAGS) -c $(SRC_OP) -o $@

# Criar pasta build
build:
	mkdir -p build

# ====== Limpeza ======

# Remove somente os .o e o binário
clean:
	rm -rf build $(TARGET)

# Remove tudo + os arquivos de dados JSON
clean-all: clean
	rm -f DADOS/*.json
