#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/IO.h"
#include "../headers/paciente.h"
#include "../headers/avl.h"
#include "../headers/fila.h"
#include "../headers/pilha.h"

// Estrutura fixa para salvar pacientes sem ponteiros
typedef struct {
    int id;
    char nome[100];
    int prioridade;
    int tamanho_historico; // Quantidade de itens no histórico
} PACIENTE_BIN;


// ============================================================================
// SALVAR
// ============================================================================
bool io_salvar(AVL* lista_pacientes, FILA* fila_espera) {
    if (!lista_pacientes || !fila_espera)
        return false;

    FILE* arq_pac = fopen("pacientes.bin", "wb");
    FILE* arq_fila = fopen("fila.bin", "wb");

    if (!arq_pac || !arq_fila) {
        if (arq_pac) fclose(arq_pac);
        if (arq_fila) fclose(arq_fila);
        return false;
    }

    // ----- SALVAR AVL -----
    int total = 0;
    PACIENTE** vetor = lista_obter_todos_pacientes(lista_pacientes, &total);

    fwrite(&total, sizeof(int), 1, arq_pac);

    for (int i = 0; i < total; i++) {
        PACIENTE* p = vetor[i];
        PACIENTE_BIN pb;

        pb.id = paciente_get_id(p);
        pb.prioridade = paciente_get_prioridade(p);

        strncpy(pb.nome, paciente_get_name(p), 99);
        pb.nome[99] = '\0';

        // Obter histórico
        PILHA* historico = paciente_get_historico(p);
        int tam_historico = 0;
        char** historico_completo = NULL;
        if (historico != NULL) {
            historico_completo = pilha_obter_historico_completo(historico, &tam_historico);
        }
        pb.tamanho_historico = tam_historico;

        // Salva dados básicos do paciente
        fwrite(&pb, sizeof(PACIENTE_BIN), 1, arq_pac);

        // Salva histórico (cada string tem tamanho máximo 101)
        for (int j = 0; j < tam_historico; j++) {
            char item[101];
            strncpy(item, historico_completo[j], 100);
            item[100] = '\0';
            fwrite(item, sizeof(char), 101, arq_pac);
        }

        // Libera histórico alocado
        if (historico_completo != NULL) {
            pilha_liberar_historico_completo(historico_completo, tam_historico);
        }
    }

    free(vetor);
    fclose(arq_pac);

    // ----- SALVAR FILAS (apenas IDs) -----
    for (int pr = 0; pr < 5; pr++) {
        int tam = 0;
        int* ids = fila_obter_ids_por_prioridade(fila_espera, pr, &tam);

        fwrite(&tam, sizeof(int), 1, arq_fila);

        for (int i = 0; i < tam; i++)
            fwrite(&ids[i], sizeof(int), 1, arq_fila);

        free(ids);  // free(NULL) é seguro
    }

    fclose(arq_fila);
    return true;
}



// ============================================================================
// CARREGAR
// ============================================================================
bool io_carregar(AVL* lista_pacientes, FILA* fila_espera) {
    if (!lista_pacientes || !fila_espera)
        return false;

    FILE* arq_pac = fopen("pacientes.bin", "rb");
    FILE* arq_fila = fopen("fila.bin", "rb");

    if (!arq_pac || !arq_fila) {
        if (arq_pac) fclose(arq_pac);
        if (arq_fila) fclose(arq_fila);
        return false;   // não existe arquivo - não modifica estruturas
    }

    // ----- CARREGAR AVL -----
    int total = 0;
    fread(&total, sizeof(int), 1, arq_pac);

    for (int i = 0; i < total; i++) {
        PACIENTE_BIN pb;

        fread(&pb, sizeof(PACIENTE_BIN), 1, arq_pac);

        PACIENTE* p = paciente_criar(pb.id, pb.nome, pb.prioridade);
        if (p == NULL) {
            continue; // Pula se não conseguir criar
        }

        // Carrega histórico se houver
        if (pb.tamanho_historico > 0 && pb.tamanho_historico <= TAM_PILHA) {
            char** historico_completo = (char**)malloc(pb.tamanho_historico * sizeof(char*));
            if (historico_completo != NULL) {
                // Lê cada item do histórico
                for (int j = 0; j < pb.tamanho_historico; j++) {
                    char item[101];
                    fread(item, sizeof(char), 101, arq_pac);
                    historico_completo[j] = (char*)malloc(101 * sizeof(char));
                    if (historico_completo[j] != NULL) {
                        strncpy(historico_completo[j], item, 100);
                        historico_completo[j][100] = '\0';
                    }
                }

                // Restaura histórico na pilha
                PILHA* historico = paciente_get_historico(p);
                if (historico != NULL) {
                    pilha_restaurar_historico(historico, historico_completo, pb.tamanho_historico);
                }

                // Libera array temporário
                for (int j = 0; j < pb.tamanho_historico; j++) {
                    free(historico_completo[j]);
                }
                free(historico_completo);
            }
        }

        lista_inserir_paciente(lista_pacientes, p);
    }

    fclose(arq_pac);

    // ----- CARREGAR FILAS -----
    for (int pr = 0; pr < 5; pr++) {
        int tam = 0;
        fread(&tam, sizeof(int), 1, arq_fila);

        for (int i = 0; i < tam; i++) {
            int id;
            fread(&id, sizeof(int), 1, arq_fila);

            PACIENTE* p = lista_buscar_paciente(lista_pacientes, id);
            if (p)
                fila_inserir_paciente(fila_espera, p);
        }
    }

    fclose(arq_fila);
    return true;
}
