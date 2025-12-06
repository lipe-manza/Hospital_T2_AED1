#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "heap.h"

// Struct do nó da heap que guarda o paciente e a seq de inserção
struct no_heap {
    PACIENTE* paciente;
    long seq; // sequência de inserção (menor = inserido antes)
};


// Estrutura da heap sequencial
struct heap_sequencial {
    NH** pacientes;    // pacientes de ponteiros para NH (alocado dinamicamente)
    int fim;        // índice do último elemento
    long prox_seq;  // contador de sequência para estabilidade
};

HS* fila_criar() {
    // aloca a struct HS corretamente
    HS* heap = malloc(sizeof(HS));
    if (!heap) return NULL;

    // aloca espaço para TAM ponteiros NH* (inicializados a NULL pelo calloc)
    heap->pacientes = calloc(TAM, sizeof(NH*));
    if (!heap->pacientes) {
        free(heap);
        return NULL;
    }

    heap->fim = -1;
    heap->prox_seq = 0;

    return heap;
}

int fila_cheia(HS* heap) {
    return (heap->fim == TAM - 1);
}

int fila_vazia(HS* heap) {
    return (heap->fim == -1);
}

static void swap_nodes(HS* heap, int i, int j) {
    NH* tmp = heap->pacientes[i];
    heap->pacientes[i] = heap->pacientes[j];
    heap->pacientes[j] = tmp;
}

/*
 compare_nodes(a, b) -> retorna 1 se a deve ficar acima de b (a "maior")
 Critério:
  - prioridade maior -> maior
  - se prioridades iguais -> menor seq (inserido antes) é considerado maior (FIFO dentro da mesma prioridade)
*/
static int compare_nodes(NH* a, NH* b) {
    if (a == NULL || b == NULL) return 0;
    int pa = paciente_get_prioridade(a->paciente);
    int pb = paciente_get_prioridade(b->paciente);
    if (pa > pb) return 1;
    if (pa < pb) return 0;
    // prioridades iguais -> quem tem seq menor foi inserido antes e deve sair primeiro
    return (a->seq < b->seq) ? 1 : 0;
}

static void fix_up(HS* heap) {
    int w = heap->fim;
    while (w > 0) {
        int parent = (w - 1) / 2;
        if (compare_nodes(heap->pacientes[w], heap->pacientes[parent])) {
            swap_nodes(heap, w, parent);
            w = parent;
        }
        else {
            break;
        }
    }
}

bool fila_inserir_paciente(HS* heap, PACIENTE* paciente) {
    if (heap == NULL || paciente == NULL) return false;
    if (fila_cheia(heap)) return false;

    NH* node = (NH*)malloc(sizeof(NH));
    if (node == NULL) return false;
    node->paciente = paciente;
    node->seq = heap->prox_seq++; // seq incremental garante estabilidade

    heap->fim++;
    heap->pacientes[heap->fim] = node;

    fix_up(heap);
    return true;
}

static void fix_down(HS* heap) {
    int w = 0;
    while ((2 * w + 1) <= heap->fim) {
        int left = 2 * w + 1;
        int right = 2 * w + 2;
        int max_child = left;

        if (right <= heap->fim && compare_nodes(heap->pacientes[right], heap->pacientes[left])) {
            max_child = right;
        }

        if (compare_nodes(heap->pacientes[max_child], heap->pacientes[w])) {
            swap_nodes(heap, w, max_child);
            w = max_child;
        }
        else {
            break;
        }
    }
}

PACIENTE* fila_remover_paciente(HS* heap) {
    if (heap == NULL || fila_vazia(heap)) return NULL;

    NH* root = heap->pacientes[0];
    PACIENTE* removed = root->paciente;
    free(root); // libera o nó (não libera o PACIENTE*)
    root = NULL;

    if (heap->fim == 0) {
        // se era o único elemento, marca empty e retorna
        heap->pacientes[0] = NULL;
        heap->fim = -1;
        return removed;
    }

    // substitui raiz pelo último e desce
    heap->pacientes[0] = heap->pacientes[heap->fim];
    heap->pacientes[heap->fim] = NULL;
    heap->fim--;
    if (!fila_vazia(heap)) // Verifica se a heap não está vazia antes de chamar fix_down
        fix_down(heap);
    return removed;
}

// Verifica se a fila contém um paciente com o ID especificado
bool fila_contem_paciente(HS* heap, int id) {
    if (heap == NULL) return false;
    for (int i = 0; i <= heap->fim; i++) {
        if (heap->pacientes[i] != NULL) {
            PACIENTE* p = heap->pacientes[i]->paciente;
            if (p != NULL && paciente_get_id(p) == id) {
                return true;
            }
        }
    }
    return false;
}

void fila_apagar(HS** heap_ptr) {
    if (heap_ptr == NULL || *heap_ptr == NULL) return;
    HS* heap = *heap_ptr;

    for (int i = 0; i <= heap->fim; i++) {
        if (heap->pacientes[i] != NULL) {

            PACIENTE* p = heap->pacientes[i]->paciente;

            // chamamos, mas NÃO acessamos p depois disso
            if (!paciente_apagar(&p)) {
                // só podemos imprimir algo SE p for != NULL
                // mas como NÃO sabemos, melhor um aviso genérico
                printf("ERRO: Não foi possível liberar um paciente da fila.\n");
            }

            // libera o nó
            free(heap->pacientes[i]);
            heap->pacientes[i] = NULL;
        }
    }

    free(heap->pacientes);
    free(heap);
    *heap_ptr = NULL;
}

/* comparador para qsort - ordena do maior para o menor conforme compare_nodes */
static int cmp_qsort(const void* a, const void* b) {
    NH* na = *(NH**)a;
    NH* nb = *(NH**)b;

    if (na == NULL && nb == NULL) return 0;
    if (na == NULL) return 1;  // nulos por último
    if (nb == NULL) return -1;

    // compare_nodes retorna 1 se na "maior" que nb
    if (compare_nodes(na, nb)) return -1; // na vem antes (maior prioridade)
    if (compare_nodes(nb, na)) return 1;  // nb vem antes
    return 0; // iguais (estabilidade já garantida pela seq)
}

void fila_imprimir(HS* heap) {
    if (heap == NULL) {
        printf("Fila de espera não existe.\n");
        return;
    }
    if (fila_vazia(heap)) {
        printf("Fila de espera está vazia.\n");
        return;
    }

    int n = heap->fim + 1;

    /* aloca cópia dinamicamente */
    NH** copia = malloc(n * sizeof(NH*));
    if (!copia) {
        printf("Erro ao alocar memória para ordenação.\n");
        return;
    }

    /* copia ponteiros (não duplica PACIENTE*, só os ponteiros para os nós) */
    memcpy(copia, heap->pacientes, n * sizeof(NH*));

    /* ordena a cópia sem modificar a heap original */
    qsort(copia, n, sizeof(NH*), cmp_qsort);

    printf("Fila de Espera (prioridade - nome - ID):\n");
    for (int i = 0; i < n; i++) {
        NH* node = copia[i];
        if (node != NULL && node->paciente != NULL) {
            int prioridade = paciente_get_prioridade(node->paciente);
            const char* nome = paciente_get_name(node->paciente);
            int id = paciente_get_id(node->paciente);
            printf("Prioridade: %d - Nome: %s - ID: %d\n", prioridade, nome, id);
        }
    }

    free(copia);
}





