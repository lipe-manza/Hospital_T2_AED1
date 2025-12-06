#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>
#include "paciente.h"

typedef struct no_heap NH;
typedef struct heap_sequencial HS;
#define TAM 1000000

HS* fila_criar();
int fila_cheia(HS* heap);
int fila_vazia(HS* heap);
bool fila_inserir_paciente(HS* heap, PACIENTE* item);
PACIENTE* fila_remover_paciente(HS* heap);
bool fila_contem_paciente(HS* heap, int id);
void fila_apagar(HS** heap);
void fila_imprimir(HS* heap);
#endif

