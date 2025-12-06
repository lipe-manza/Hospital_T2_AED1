#ifndef PACIENTE_H
#define PACIENTE_H

#include <stdbool.h>


typedef struct paciente PACIENTE;

PACIENTE* paciente_criar(int id, const char* name, int prioridade);
bool paciente_apagar(PACIENTE** paciente);
void paciente_imprimir(PACIENTE* paciente);
int paciente_get_id(PACIENTE* paciente);
const char* paciente_get_name(PACIENTE* paciente);
int paciente_get_prioridade(PACIENTE* paciente);

#endif