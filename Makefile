CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address -fno-omit-frame-pointer -Iheaders
LDFLAGS = -fsanitize=address

SRC_MAIN = main.c
SRC_PACIENTE = src/paciente.c
SRC_PILHA = src/pilha.c
SRC_AVL = src/avl.c
SRC_FILA = src/fila.c
SRC_OP = src/op.c
SRC_IO = src/IO.c

OBJS = build/main.o build/paciente.o build/pilha.o build/avl.o build/fila.o build/op.o build/IO.o

TARGET = programa

# ====== Build padrão ======
all: build data $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# ====== Compilação dos módulos ======
build/main.o: $(SRC_MAIN) headers/paciente.h headers/avl.h headers/fila.h headers/op.h
	$(CC) $(CFLAGS) -c $(SRC_MAIN) -o $@

build/paciente.o: $(SRC_PACIENTE) headers/paciente.h headers/pilha.h
	$(CC) $(CFLAGS) -c $(SRC_PACIENTE) -o $@

build/pilha.o: $(SRC_PILHA) headers/pilha.h
	$(CC) $(CFLAGS) -c $(SRC_PILHA) -o $@

build/avl.o: $(SRC_AVL) headers/avl.h headers/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_AVL) -o $@

build/fila.o: $(SRC_FILA) headers/fila.h headers/paciente.h
	$(CC) $(CFLAGS) -c $(SRC_FILA) -o $@

build/op.o: $(SRC_OP) headers/op.h headers/paciente.h headers/avl.h headers/fila.h
	$(CC) $(CFLAGS) -c $(SRC_OP) -o $@

build/IO.o: $(SRC_IO) headers/IO.h headers/paciente.h headers/avl.h headers/fila.h headers/pilha.h
	$(CC) $(CFLAGS) -c $(SRC_IO) -o $@

# Criar pasta build
build:
	mkdir -p build

# Criar pasta data
data:
	mkdir -p data

run: all
	./$(TARGET)

# ====== Limpeza ======
clean:
	rm -rf build $(TARGET)

clean-all: clean
	rm -rf data
