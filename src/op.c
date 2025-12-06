#include <stdio.h>
#include <string.h>
#include "../headers/op.h"

void call_options(AVL* lista_de_pacientes, FILA* fila_de_espera, int choice) {
    switch (choice)
    {
    case 1: // Registrar Paciente
    {
        int id;
        char nome[100];
        int prioridade; // prioridade do paciente 0-4 (Não Urgência - Pouco Urgente - Urgente - Muito Urgente - Emergência)

        printf("Insira o ID do paciente: ");
        scanf("%d", &id);
        while (id < 0) {// verifica se o ID é positivo
            printf("ERRO: ID inválido. Por favor, insira um ID positivo.\n");
            printf("Insira o ID do paciente: ");
            scanf("%d", &id);
        }

        // Verificar se ID já existe
        PACIENTE* paciente_antigo = lista_buscar_paciente(lista_de_pacientes, id);
        if (paciente_antigo != NULL) { // Verifica se já existe um paciente com esse id na lista de pacientes
            if (fila_contem_paciente(fila_de_espera, id)) { // Verifica se o paciente já está na fila de espera
                printf("ERRO: Paciente com ID %d já está registrado e já está na fila de espera.\n", id);
                break;
            }
            else {
                fila_inserir_paciente(fila_de_espera, paciente_antigo);// insere o paciente ja registrado na fila de espera
                printf("ERRO: Paciente com ID %d já está registrado.\n", id);
                printf("Paciente %s ID(%d) adicionado a fila de espera", paciente_get_name(paciente_antigo), id);
                break;
            }
        }
        printf("Insira o nome do paciente:");
        scanf(" %[^\n]", nome);



        // Verificar se a fila está cheia
        if (fila_cheia(fila_de_espera)) {
            printf("ERRO: Fila de espera está cheia. Não é possível registrar mais pacientes no momento.\n");
            break;
        }
        // Solicitar prioridade do paciente
        printf("Insira a prioridade do paciente (0-Não Urgência, 1-Pouco Urgente, 2-Urgente, 3-Muito Urgente, 4-Emergência):\n");
        scanf("%d", &prioridade);
        while (prioridade < 0 || prioridade > 4) { // verifica se a prioridade é válida
            printf("ERRO: Prioridade inválida. Por favor, insira uma prioridade entre 0 e 4.\n");
            printf("Insira a prioridade do paciente (0-Não Urgência, 1-Pouco Urgente, 2-Urgente, 3-Muito Urgente, 4-Emergência):\n");
            scanf("%d", &prioridade);
        }

        PACIENTE* novo_paciente = paciente_criar(id, nome, prioridade);
        if (novo_paciente == NULL) {
            printf("ERRO: Não foi possível criar o paciente.\n");
            break;
        }

        // Tenta inserir o paciente na lista e se não conseguir printa erro
        if (!lista_inserir_paciente(lista_de_pacientes, novo_paciente)) {
            printf("ERRO: Não foi possível inserir o paciente na lista de pacientes.\n");
            paciente_apagar(&novo_paciente); // libera memória se não conseguir inserir na lista
            break;
        }

        // Tenta inserir o paciente na fila e se não conseguir printa erro
        if (!fila_inserir_paciente(fila_de_espera, novo_paciente)) {
            printf("ERRO: Não foi possível adicionar o paciente à fila de espera.\n");
            lista_remover_paciente(lista_de_pacientes, id); // remove da lista se não conseguir inserir na fila
            novo_paciente = NULL;
            break;
        }

        printf("\nPaciente %s registrado com ID %d e adicionado à fila de espera.\n", nome, id);

        break;

    }
    case 2: // Remover paciente
    {
        printf("Insira o ID do paciente que deseja remover: ");
        int id_remover;
        scanf("%d", &id_remover);

        // Verificar se o paciente existe
        PACIENTE* paciente_obito = lista_buscar_paciente(lista_de_pacientes, id_remover);
        if (paciente_obito == NULL) {
            printf("ERRO: Paciente com ID %d não encontrado no sistema.\n", id_remover);
            break;
        }

        // Verificar se o paciente ainda está na fila de espera
        if (fila_contem_paciente(fila_de_espera, id_remover)) {
            printf("ERRO: Paciente com ID %d ainda está na fila de espera. Pacientes na fila não podem falecer!\n", id_remover);
            break;
        }

        char nome_removido[100];
        strncpy(nome_removido, paciente_get_name(paciente_obito), sizeof(nome_removido));
        // Remover paciente da lista e liberar memória
        if (lista_remover_paciente(lista_de_pacientes, id_remover)) {
            printf("Paciente %s id: %d removido do sistema.\n", nome_removido, id_remover);
        }
        else {
            printf("ERRO: Não foi possível remover o paciente com ID %d.\n", id_remover);
        }
        break;
    }
    case 3: // Listar pacientes
    {
        printf("Lista de Pacientes Registrados:\n\n");
        lista_imprimir(lista_de_pacientes);
      
        break;
    }
    case 4: { // Buscar paciente por ID
        int id_buscar;
        printf("Insira o ID do paciente que deseja buscar: ");
        scanf("%d", &id_buscar);

        PACIENTE* paciente_encontrado = lista_buscar_paciente(lista_de_pacientes, id_buscar);
        if (paciente_encontrado != NULL) {
            printf("Paciente encontrado:\n");
            paciente_imprimir(paciente_encontrado);
        }
        else {
            printf("Paciente com ID %d não encontrado na lista.\n", id_buscar);
        }
        break;

    }
    case 5: { // Mostrar fila de espera
        fila_imprimir(fila_de_espera);
        break;
    }
    case 6:// Dar alta ao paciente 
    {
        PACIENTE* paciente_atendimento = fila_remover_paciente(fila_de_espera); // Retira o primeiro da fila da triagem
        if (paciente_atendimento != NULL) { // Verifica se o paciente existe e printa que foi chamado
            printf("Foi dada alta ao paciente %s (ID %d) .\n",
                paciente_get_name(paciente_atendimento),
                paciente_get_id(paciente_atendimento));
        }
        else {
            printf("ERRO: Nenhum paciente na fila de espera.\n");
        }
        break;
    }
    case 7: // Sair
        printf("Encerrando o sistema...\n");
        break;
    default:
        printf("Opção inválida! Por favor, escolha uma opção entre 0 e 8.\n");
        break;
    }
}