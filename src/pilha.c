#include "../headers/pilha.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Pilha sequencial 
struct pilha {
    char historico[TAM_PILHA][101]; // Array de strings - 10 strings de tamanho 100 cada uma
    int topo; // Topo da pilha
};

// Função que cria a pilha
PILHA* pilha_criar(void) {
    PILHA* p = (PILHA*)malloc(sizeof(PILHA));
    if (p != NULL) {
        p->topo = -1; // Seta o topo para -1 para indicar que está vazia
        // Inicializa o array
        for (int i = 0; i < TAM_PILHA; i++) {
            p->historico[i][0] = '\0';
        }
        return p;
    }
    return NULL;
}

void pilha_apagar(PILHA** p) {
    if (p == NULL || *p == NULL) {
        return;
    }
    memset((*p)->historico, 0, sizeof((*p)->historico));
    (*p)->topo = -1;
    free(*p);
    *p = NULL;
}

bool pilha_vazia(PILHA* p) {
    if (p == NULL)
        return true;

    return (p->topo == -1);
}

bool pilha_cheia(PILHA* p) {
    if (p == NULL) return true;

    return (p->topo == TAM_PILHA - 1);
}

int pilha_tamanho(PILHA* p) {
    if (p == NULL) return -1;

    return (p->topo) + 1;
}

int pilha_topo(PILHA* p) {
    if (p == NULL) return 0;

    if (p->topo == -1) return 0;

    return p->topo;
}

// Empilha o procedimento medico na pilha
bool pilha_empilhar(PILHA* p, char* medicamento) {
    if (p == NULL || medicamento == NULL) return false;

    if (pilha_cheia(p)) return false;

    p->topo++;
    strncpy(p->historico[p->topo], medicamento, 100);
    p->historico[p->topo][100] = '\0'; // Garante terminação nula
    return true;
}

// Desempilha o procedimento medico na pilha e retorna o procedimento
char* pilha_desempilhar(PILHA* p) {
    if (p == NULL) return NULL;

    if (pilha_vazia(p)) return NULL;

    static char historico[101];
    strncpy(historico, p->historico[p->topo], 100);
    historico[100] = '\0';
    memset(p->historico[p->topo], 0, 101);
    p->topo--;
    return historico;
}

// Imprime a pilha
void pilha_imprimir(PILHA* p) {
    if (p == NULL || pilha_vazia(p)) {
        printf("Histórico vazio.\n");
        return;
    }

    for (int i = p->topo; i >= 0; i--) {
        printf("%d°- %s\n", i + 1, p->historico[i]);
    }
}

// Função auxiliar para obter o histórico completo (para uso em IO)
// Retorna array de strings e o tamanho
char** pilha_obter_historico_completo(PILHA* p, int* tamanho) {
    if (p == NULL || tamanho == NULL) {
        if (tamanho) *tamanho = 0;
        return NULL;
    }

    int tam = pilha_tamanho(p);
    if (tam <= 0) {
        *tamanho = 0;
        return NULL;
    }

    char** historico = (char**)malloc(tam * sizeof(char*));
    if (historico == NULL) {
        *tamanho = 0;
        return NULL;
    }

    // Copia do topo para a base (ordem LIFO)
    for (int i = 0; i < tam; i++) {
        historico[i] = (char*)malloc(101 * sizeof(char));
        if (historico[i] == NULL) {
            // Libera o que foi alocado
            for (int j = 0; j < i; j++) {
                free(historico[j]);
            }
            free(historico);
            *tamanho = 0;
            return NULL;
        }
        strncpy(historico[i], p->historico[i], 100);
        historico[i][100] = '\0';
    }

    *tamanho = tam;
    return historico;
}

// Função auxiliar para limpar o histórico alocado por pilha_obter_historico_completo
void pilha_liberar_historico_completo(char** historico, int tamanho) {
    if (historico == NULL) return;

    for (int i = 0; i < tamanho; i++) {
        free(historico[i]);
    }
    free(historico);
}

// Função para restaurar histórico a partir de um array
bool pilha_restaurar_historico(PILHA* p, char** historico, int tamanho) {
    if (p == NULL || historico == NULL || tamanho < 0) return false;

    // Limpa a pilha atual
    p->topo = -1;
    for (int i = 0; i < TAM_PILHA; i++) {
        p->historico[i][0] = '\0';
    }

    // Restaura o histórico (do mais antigo para o mais recente para manter ordem LIFO)
    for (int i = 0; i < tamanho && i < TAM_PILHA; i++) {
        strncpy(p->historico[i], historico[i], 100);
        p->historico[i][100] = '\0';
        p->topo = i;
    }

    return true;
}

