#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "paciente.h"

typedef struct sistema_filas FILA;
#define TAM 1000000

FILA* fila_criar();
int fila_cheia(FILA* fila);
int fila_vazia(FILA* fila);
bool fila_inserir_paciente(FILA* fila, PACIENTE* item);
PACIENTE* fila_remover_paciente(FILA* fila);
bool fila_contem_paciente(FILA* fila, int id);
void fila_apagar(FILA** fila);
void fila_imprimir(FILA* fila);

// Função auxiliar para obter IDs da fila por prioridade (para uso em IO)
// Retorna array de IDs para uma prioridade específica
int* fila_obter_ids_por_prioridade(FILA* fila, int prioridade, int* tamanho);
#endif

