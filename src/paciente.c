#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/paciente.h"


// Struct paciente que armazena o id nome e um historico de cada paciente
struct paciente {
    int id;
    char nome[100];
    int prioridade; // 0-4 , (Não Urgência - Pouco Urgente - Urgente - Muito Urgente - Emergência)
};

// Função que cria o paciente com seu id e nome
PACIENTE* paciente_criar(int id, const char* nome, int prioridade) {
    PACIENTE* p = (PACIENTE*)malloc(sizeof(PACIENTE));
    if (p != NULL) {
        p->id = id;
        p->prioridade = prioridade;
        strncpy(p->nome, nome, sizeof(p->nome) - 1);
        p->nome[sizeof(p->nome) - 1] = '\0'; // evita estouro
    }
    return p;
}


bool paciente_apagar(PACIENTE** paciente) {
    if (paciente != NULL && *paciente != NULL) {
        free(*paciente);
        *paciente = NULL;
        return true;
    }
    return false;
}

// Função para retornar o id do paciente
int paciente_get_id(PACIENTE* p) {
    if (p != NULL) {
        return p->id;
    }
    return -1; // Indica erro
}

int paciente_get_prioridade(PACIENTE* p) {
    if (p != NULL) {
        return p->prioridade;
    }
    return -1; // Indica erro
}

// Função para retornar o nome do paciente
const char* paciente_get_name(PACIENTE* p) {
    if (p != NULL) {
        return p->nome;
    }
    return NULL; // Indica erro
}

// Função para imprimir o ID e o nome do paciente 
void paciente_imprimir(PACIENTE* p) {
    if (p != NULL) {
        printf("%d - %s\n", p->id, p->nome);
    }
}


