#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/fila.h"

// Struct do nó de uma fila simples (FIFO)
struct no_fila {
    PACIENTE* paciente;
    struct no_fila* proximo;
};

// Estrutura de uma fila simples (FIFO)
struct fila_simples {
    struct no_fila* inicio;  // primeiro da fila
    struct no_fila* fim;     // último da fila
    int tamanho;             // quantidade de elementos
};

// Estrutura principal que contém 5 filas (uma para cada prioridade 0-4)
struct sistema_filas {
    struct fila_simples filas[5];  // filas[0] = prioridade 0, filas[4] = prioridade 4
    int total_pacientes;           // total de pacientes em todas as filas
};

// Inicializa uma fila simples (privada)
static void fila_simples_inicializar(struct fila_simples* fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
    fila->tamanho = 0;
}

// Cria e inicializa uma nova fila de prioridades (pública)
FILA* fila_criar() {
    FILA* fila = malloc(sizeof(FILA));
    if (!fila) return NULL;

    // Inicializa todas as 5 filas
    for (int i = 0; i < 5; i++) {
        fila_simples_inicializar(&fila->filas[i]);
    }

    fila->total_pacientes = 0;
    return fila;
}

// Verifica se a fila está cheia (pública)
int fila_cheia(FILA* fila) {
    // Verifica se o total de pacientes atingiu o limite
    return (fila != NULL && fila->total_pacientes >= TAM);
}

// Verifica se a fila está vazia (pública)
int fila_vazia(FILA* fila) {
    if (fila == NULL) return 1;
    return (fila->total_pacientes == 0);
}

// Insere um paciente em uma fila simples específica (FIFO) (privada)
static bool fila_simples_inserir(struct fila_simples* fila, PACIENTE* paciente) {
    if (fila == NULL || paciente == NULL) return false;

    // Cria um novo nó
    struct no_fila* novo_no = malloc(sizeof(struct no_fila));
    if (novo_no == NULL) return false;

    // Inicializa o nó
    novo_no->paciente = paciente;
    novo_no->proximo = NULL;

    // Insere no inicio da fila se estiver vazia e no final caso contrário
    if (fila->fim == NULL) {
        // Fila vazia
        fila->inicio = novo_no;
        fila->fim = novo_no;
    }
    else {
        // Adiciona no fim
        fila->fim->proximo = novo_no;
        fila->fim = novo_no;
    }

    // Incrementa o tamanho da fila
    fila->tamanho++;
    return true;
}

// Remove um paciente de uma fila simples específica (FIFO) (privada)
static PACIENTE* fila_simples_remover(struct fila_simples* fila) {
    if (fila == NULL || fila->inicio == NULL) return NULL;

    // Remove o nó do início da fila
    struct no_fila* no_removido = fila->inicio;
    PACIENTE* paciente = no_removido->paciente;

    // Atualiza o início da fila
    fila->inicio = fila->inicio->proximo;
    if (fila->inicio == NULL) {
        // Fila ficou vazia
        fila->fim = NULL;
    }

    // Decrementa o tamanho da fila
    fila->tamanho--;
    // Libera o nó removido
    free(no_removido);

    return paciente;
}

// Verifica se uma fila simples está vazia (privada)
static bool fila_simples_vazia(struct fila_simples* fila) {
    return (fila == NULL || fila->inicio == NULL);
}

// Insere um paciente na fila de acordo com sua prioridade (pública)
bool fila_inserir_paciente(FILA* fila, PACIENTE* paciente) {
    if (fila == NULL || paciente == NULL) return false;
    if (fila_cheia(fila)) return false;

    int prioridade = paciente_get_prioridade(paciente);
    if (prioridade < 0 || prioridade > 4) return false;

    if (fila_simples_inserir(&fila->filas[prioridade], paciente)) {
        fila->total_pacientes++;
        return true;
    }
    return false;
}

// Remove e retorna o paciente com a maior prioridade (FIFO entre mesmos níveis) (pública)
PACIENTE* fila_remover_paciente(FILA* fila) {
    if (fila == NULL || fila_vazia(fila)) return NULL;

    // Tenta remover começando da prioridade mais alta (4) até a mais baixa (0)
    for (int prioridade = 4; prioridade >= 0; prioridade--) {
        if (!fila_simples_vazia(&fila->filas[prioridade])) {
            PACIENTE* paciente = fila_simples_remover(&fila->filas[prioridade]);
            fila->total_pacientes--;
            return paciente;
        }
    }

    return NULL;
}

// Verifica se a fila contém um paciente com o ID especificado
bool fila_contem_paciente(FILA* fila, int id) {
    if (fila == NULL) return false;

    // Procura em todas as 5 filas
    for (int prioridade = 0; prioridade < 5; prioridade++) {
        // Percorre a fila simples atual
        struct no_fila* atual = fila->filas[prioridade].inicio;
        while (atual != NULL) {
            if (atual->paciente != NULL && paciente_get_id(atual->paciente) == id) {
                return true;
            }
            atual = atual->proximo;
        }
    }
    return false;
}

// Apaga a fila e libera toda a memória associada (pública)
void fila_apagar(FILA** fila_ptr) {
    // Verifica ponteiro nulo
    if (fila_ptr == NULL || *fila_ptr == NULL) return;
    FILA* fila = *fila_ptr;

    // Limpa todas as 5 filas e libera os pacientes
    for (int prioridade = 0; prioridade < 5; prioridade++) {
        struct no_fila* atual = fila->filas[prioridade].inicio;
        while (atual != NULL) {
            struct no_fila* proximo = atual->proximo;
            // Libera cada nó
            free(atual);
            atual = proximo;
        }
        fila_simples_inicializar(&fila->filas[prioridade]);
    }

    // Libera a estrutura da fila principal
    free(fila);
    *fila_ptr = NULL;
}

// Imprime o conteúdo da fila (pública)
void fila_imprimir(FILA* fila) {
    if (fila == NULL) {
        printf("Fila de espera não existe.\n");
        return;
    }
    if (fila_vazia(fila)) {
        printf("Fila de espera está vazia.\n");
        return;
    }

    // Nomes das prioridades para impressão
    const char* nomes_prioridade[] = {
        "Não Urgência",
        "Pouco Urgente",
        "Urgente",
        "Muito Urgente",
        "Emergência"
    };

    printf("Fila de Espera (prioridade - nome - ID):\n");

    // Imprime começando da prioridade mais alta (4) até a mais baixa (0)
    for (int prioridade = 4; prioridade >= 0; prioridade--) {
        struct no_fila* atual = fila->filas[prioridade].inicio;
        while (atual != NULL) {
            if (atual->paciente != NULL) {
                // Obtém nome e ID do paciente
                const char* nome = paciente_get_name(atual->paciente);
                int id = paciente_get_id(atual->paciente);
                // Imprime informações do paciente
                printf("Prioridade: %d (%s) - Nome: %s - ID: %d\n",
                    prioridade, nomes_prioridade[prioridade], nome, id);
            }
            atual = atual->proximo;
        }
    }
}

// Obtém IDs da fila para uma prioridade específica (pública)
int* fila_obter_ids_por_prioridade(FILA* fila, int prioridade, int* tamanho) {
    if (fila == NULL || tamanho == NULL || prioridade < 0 || prioridade > 4) {
        if (tamanho != NULL) *tamanho = 0;
        return NULL;
    }
    // Conta quantos pacientes existem na fila da prioridade especificada
    int count = fila->filas[prioridade].tamanho;
    if (count == 0) {
        *tamanho = 0;
        return NULL;
    }

    // Aloca array para os IDs
    int* ids = (int*)malloc(count * sizeof(int));
    if (ids == NULL) {
        *tamanho = 0;
        return NULL;
    }

    // Preenche o array com os IDs dos pacientes na fila da prioridade especificada
    int index = 0;
    struct no_fila* atual = fila->filas[prioridade].inicio;
    while (atual != NULL) {
        if (atual->paciente != NULL) {
            ids[index++] = paciente_get_id(atual->paciente);
        }
        atual = atual->proximo;
    }

    // Ajusta o tamanho real retornado
    *tamanho = index;
    return ids;
}





