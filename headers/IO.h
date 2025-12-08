#ifndef IO_H
#define IO_H

#include "avl.h"
#include "fila.h"
#include <stdbool.h>

// Funções para salvar e carregar dados de pacientes e filas
bool io_salvar(AVL* lista_pacientes, FILA* fila_espera); // Salva os pacientes da AVL e IDs da fila em arquivos
bool io_carregar(AVL* lista_pacientes, FILA* fila_espera); // Carrega os pacientes dos arquivos na AVL e reconstrói a fila

#endif
