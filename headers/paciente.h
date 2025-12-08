#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdbool.h>
#include "pilha.h"

typedef struct paciente PACIENTE;

// Funções para criação e destruição de pacientes
PACIENTE* paciente_criar(int id, const char* name, int prioridade); // Cria um novo paciente
bool paciente_apagar(PACIENTE** paciente); // Apaga um paciente

// FUnções para manipulação de pacientes
void paciente_imprimir(PACIENTE* paciente); // Imprime as informações do paciente
int paciente_get_id(PACIENTE* paciente); // Retorna o ID do paciente
const char* paciente_get_name(PACIENTE* paciente);// Retorna o nome do paciente
int paciente_get_prioridade(PACIENTE* paciente); // Retorna a prioridade do paciente

// Funções para histórico
bool paciente_add_medicamento(PACIENTE* paciente, char* medicamento); // Adiciona um medicamento ao histórico
bool paciente_remover_medicamento(PACIENTE* paciente); // Remove o último medicamento do histórico
char* paciente_retirar_ultimo_medicamento(PACIENTE* paciente); // Retira e retorna o último medicamento do histórico
bool paciente_imprimir_historico(PACIENTE* paciente); // Imprime o histórico de medicamentos
PILHA* paciente_get_historico(PACIENTE* paciente); // Retorna a pilha do histórico de medicamentos

#endif