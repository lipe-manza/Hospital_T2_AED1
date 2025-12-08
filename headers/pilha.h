#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include <stdio.h>

#define TAM_PILHA 10

typedef struct pilha PILHA;

PILHA* pilha_criar(void);
void pilha_apagar(PILHA** pilha);
bool pilha_vazia(PILHA* pilha);
bool pilha_cheia(PILHA* pilha);
int pilha_tamanho(PILHA* pilha);
int pilha_topo(PILHA* pilha);
bool pilha_empilhar(PILHA* pilha, char* medicamento);
char* pilha_desempilhar(PILHA* pilha);
void pilha_imprimir(PILHA* pilha);

// Funções auxiliares para IO
char** pilha_obter_historico_completo(PILHA* p, int* tamanho);
void pilha_liberar_historico_completo(char** historico, int tamanho);
bool pilha_restaurar_historico(PILHA* p, char** historico, int tamanho);

#endif

