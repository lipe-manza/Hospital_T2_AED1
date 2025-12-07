#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/IO.h"
#include "../headers/paciente.h"
#include "../headers/avl.h"
#include "../headers/fila.h"

// Estrutura fixa para salvar pacientes sem ponteiros
typedef struct {
    int id;
    char nome[100];
    int prioridade;
} PACIENTE_BIN;


// ============================================================================
// SALVAR
// ============================================================================
bool io_salvar(AVL* lista_pacientes, FILA* fila_espera) {
    if (!lista_pacientes || !fila_espera)
        return false;

    FILE* arq_pac = fopen("data/pacientes.bin", "wb");
    FILE* arq_fila = fopen("data/fila.bin", "wb");

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

        fwrite(&pb, sizeof(PACIENTE_BIN), 1, arq_pac);
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
bool io_carregar(AVL** lista_pacientes, FILA** fila_espera) {
    if (!lista_pacientes || !fila_espera)
        return false;

    FILE* arq_pac = fopen("data/pacientes.bin", "rb");
    FILE* arq_fila = fopen("data/fila.bin", "rb");

    if (!arq_pac || !arq_fila) {
        if (arq_pac) fclose(arq_pac);
        if (arq_fila) fclose(arq_fila);
        return false;   // não existe arquivo
    }

    // Criar estruturas novas
    *lista_pacientes = lista_criar();
    *fila_espera = fila_criar();

    if (!*lista_pacientes || !*fila_espera) {
        if (*lista_pacientes) lista_apagar(lista_pacientes);
        if (*fila_espera) fila_apagar(fila_espera);
        fclose(arq_pac);
        fclose(arq_fila);
        return false;
    }

    // ----- CARREGAR AV L -----
    int total = 0;
    fread(&total, sizeof(int), 1, arq_pac);

    for (int i = 0; i < total; i++) {
        PACIENTE_BIN pb;

        fread(&pb, sizeof(PACIENTE_BIN), 1, arq_pac);

        PACIENTE* p = paciente_criar(pb.id, pb.nome, pb.prioridade);
        lista_inserir_paciente(*lista_pacientes, p);
    }

    fclose(arq_pac);

    // ----- CARREGAR FILAS -----
    for (int pr = 0; pr < 5; pr++) {
        int tam = 0;
        fread(&tam, sizeof(int), 1, arq_fila);

        for (int i = 0; i < tam; i++) {
            int id;
            fread(&id, sizeof(int), 1, arq_fila);

            PACIENTE* p = lista_buscar_paciente(*lista_pacientes, id);
            if (p)
                fila_inserir_paciente(*fila_espera, p);
        }
    }

    fclose(arq_fila);
    return true;
}
