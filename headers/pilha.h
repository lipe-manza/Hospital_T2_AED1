#ifndef PILHA_H
#define PILHA_H

#include <stdbool.h>
#include <stdio.h>

#define TAM_PILHA 10

typedef struct pilha PILHA;

// Funções básicas da pilha
PILHA* pilha_criar(void); // Cria e inicializa a pilha
void pilha_apagar(PILHA** pilha); // Apaga a pilha e libera a memória

// Funções de consulta
bool pilha_vazia(PILHA* pilha); // Verifica se a pilha está vazia
bool pilha_cheia(PILHA* pilha); // Verifica se a pilha está cheia
int pilha_tamanho(PILHA* pilha); // Retorna o número de elementos na pilha
int pilha_topo(PILHA* pilha); // Retorna o índice do topo da pilha

// Funções de modificação
bool pilha_empilhar(PILHA* pilha, char* medicamento); // Adiciona um medicamento/procedimento
char* pilha_desempilhar(PILHA* pilha); // Remove e retorna o medicamento/procedimento do topo
void pilha_imprimir(PILHA* pilha); // Imprime o conteúdo da pilha

// Funções auxiliares para IO
char** pilha_obter_historico_completo(PILHA* p, int* tamanho); // Retorna um array com todo o histórico
void pilha_liberar_historico_completo(char** historico, int tamanho); // Libera o array retornado por pilha_obter_historico_completo
bool pilha_restaurar_historico(PILHA* p, char** historico, int tamanho);// Restaura o histórico a partir de um array

#endif

