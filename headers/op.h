#ifndef OP_H
#define OP_H

#include "avl.h"
#include "fila.h"

// Função para chamar as operações do sistema com base na escolha do usuário
void call_options(AVL* lista_de_pacientes, FILA* fila_de_espera ,int choice);

#endif
