#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#include "paciente.h"

typedef struct sistema_filas FILA;
// Máximo de pacientes na fila (Arbitrário)
#define TAM 1000000

// Cria e inicializa uma nova fila de prioridades
FILA* fila_criar();
// Verifica se a fila está cheia
int fila_cheia(FILA* fila);
// Verifica se a fila está vazia
int fila_vazia(FILA* fila);
// Insere um paciente na fila de acordo com sua prioridade
bool fila_inserir_paciente(FILA* fila, PACIENTE* item);
// Remove e retorna o paciente com a maior prioridade (FIFO entre mesmos níveis)
PACIENTE* fila_remover_paciente(FILA* fila);
// Verifica se a fila contém um paciente com o ID especificado
bool fila_contem_paciente(FILA* fila, int id);
// Apaga a fila e libera toda a memória associada
void fila_apagar(FILA** fila);
// Imprime o conteúdo da fila (para depuração)
void fila_imprimir(FILA* fila);

// Função auxiliar para obter IDs da fila por prioridade (para uso em IO)
// Retorna array de IDs para uma prioridade específica
int* fila_obter_ids_por_prioridade(FILA* fila, int prioridade, int* tamanho);
#endif

