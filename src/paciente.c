#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/paciente.h"


// Struct paciente que armazena o id nome e um historico de cada paciente
struct paciente {
    int id;
    char nome[100];
    int prioridade; // 0-4 , (Não Urgência - Pouco Urgente - Urgente - Muito Urgente - Emergência)
    PILHA* historico; // Ponteiro para a pilha que armazena o histórico do paciente
};

// Função que cria o paciente com seu id e nome (publica)
PACIENTE* paciente_criar(int id, const char* nome, int prioridade) {
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL) {
        p->id = id;
        p->prioridade = prioridade;
        strncpy(p->nome, nome, sizeof(p->nome) - 1);
        p->nome[sizeof(p->nome) - 1] = '\0'; // evita estouro
        p->historico = pilha_criar(); // Cria o histórico do paciente
        if (p->historico == NULL) {
            free(p);
            return NULL;
        }
    }
    return p;
}

// Função que apaga o paciente (publica)
bool paciente_apagar(PACIENTE** paciente) {
    if (paciente != NULL && *paciente != NULL) {
        if ((*paciente)->historico != NULL) {
            pilha_apagar(&(*paciente)->historico);
        }
        free(*paciente);
        *paciente = NULL;
        return true;
    }
    return false;
}

// Função para retornar o id do paciente (publica)
int paciente_get_id(PACIENTE* p) {
    if (p != NULL) {
        return p->id;
    }
    return -1; // Indica erro
}

// Função para retornar a prioridade do paciente (publica)
int paciente_get_prioridade(PACIENTE* p) {
    if (p != NULL) {
        return p->prioridade;
    }
    return -1; // Indica erro
}

// Função para retornar o nome do paciente (publica)
const char* paciente_get_name(PACIENTE* p) {
    if (p != NULL) {
        return p->nome;
    }
    return NULL; // Indica erro
}

// Função para imprimir apenas o ID e nome do paciente (publica)
void paciente_imprimir(PACIENTE* p) {
    if (p != NULL) {
        printf("%d - %s\n", p->id, p->nome);
    }
}

// Função para adicionar um medicamento/procedimento no historico do paciente(pilha) (publica)
bool paciente_add_medicamento(PACIENTE* p, char* medicamento) {
    if (p != NULL && medicamento != NULL && p->historico != NULL) {
        return pilha_empilhar(p->historico, medicamento);
    }
    return false;
}

// Função para remover um medicamento/procedimento no historico do paciente(pilha)(publica)
bool paciente_remover_medicamento(PACIENTE* p) {
    if (p != NULL && p->historico != NULL) {
        char* medicamento = pilha_desempilhar(p->historico);
        return (medicamento != NULL);
    }
    return false;
}

// Função para remover um medicamento/procedimento no historico do paciente(pilha) e retorna o procedimento (publica)
char* paciente_retirar_ultimo_medicamento(PACIENTE* p) {
    if (p != NULL && p->historico != NULL) {
        return pilha_desempilhar(p->historico);
    }
    return NULL;
}

// Imprime o histórico médico do paciente (publica)
bool paciente_imprimir_historico(PACIENTE* p) {
    if (p != NULL && p->historico != NULL) {
        pilha_imprimir(p->historico);
        return true;
    }
    return false;
}

// Função que retorna o historico do paciente (ponteiro para pilha) (publica)
PILHA* paciente_get_historico(PACIENTE* p) {
    if (p != NULL) {
        return p->historico;
    }
    return NULL;
}


