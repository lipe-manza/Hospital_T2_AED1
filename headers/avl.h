#ifndef BINARY_BALANCED_SEARCH_TREE_H
#define BINARY_BALANCED_SEARCH_TREE_H

#include <stdbool.h>
#include "paciente.h"

typedef struct no NO;
typedef struct avl AVL;

// Cria a árvore vazia
AVL* lista_criar(void);

// Insere um novo paciente na árvore
bool lista_inserir_paciente(AVL* T, PACIENTE* paciente);

// Remove paciente por chave (id)
bool lista_remover_paciente(AVL* T, int id);

// Busca paciente por chave (id)
PACIENTE* lista_buscar_paciente(AVL* T, int id);

// Deleta a árvore liberando memória (passa ponteiro para NULL)
void lista_apagar(AVL** T);

// Imprime a árvore em ordem (in-order traversal)
void lista_imprimir(AVL* T);

// Função auxiliar para obter todos os pacientes da AVL (para uso em IO)
// Retorna array de ponteiros para pacientes e o tamanho
PACIENTE** lista_obter_todos_pacientes(AVL* T, int* tamanho);

#endif
