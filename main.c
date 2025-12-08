#include <stdio.h>
#include "headers/avl.h"
#include "headers/fila.h"
#include "headers/op.h"
#include "headers/IO.h"

// Função para carregar o display de escolhas no terminal, pega a escolha e retorna ela 
int display_menu() {
    int choice;
    printf("\n");
    printf("==============================================\n");
    printf("Menu:\n");
    printf("1. Registrar Paciente\n");
    printf("2. Remover paciente\n");
    printf("3. Listar pacientes\n");
    printf("4. Buscar paciente por id\n");
    printf("5. Mostrar fila de espera\n");
    printf("6. Dar alta ao paciente\n");
    printf("7. Adicionar medicamento/procedimento\n");
    printf("8. Sair\n");
    printf("Escolha uma opção: ");
    if (scanf("%d", &choice) != 1) { // verifica se a entrada é válida
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        return 0;
    }
    printf("\n==============================================\n");
    printf("\n");
    return choice;
}

int main() {
    int choice;

    // Criar estruturas
    FILA* fila_de_espera = fila_criar();
    AVL* lista_de_pacientes = lista_criar();
    
    if (!lista_de_pacientes || !fila_de_espera) {
        printf("ERRO: Não foi possível iniciar.\n");
        return 1;
    }
    
    // Carregar dados salvos
    printf("===========================================\n");
    printf("   Sistema de Gestão Hospitalar - PS\n");
    printf("===========================================\n\n");
    printf("Carregando dados salvos...\n");
    if (io_carregar(lista_de_pacientes, fila_de_espera)) {
        printf("Dados carregados com sucesso!\n\n");
    }
    else {
        printf("Nenhum dado salvo encontrado. Sistema iniciado vazios.\n\n");
    }


    // Inicio do menu e dos switchs
    do {
        choice = display_menu();
        call_options(lista_de_pacientes, fila_de_espera, choice);

    } while (choice != 8);

    // Salvar dados antes de sair
    printf("\nSalvando dados...\n");
    if (io_salvar(lista_de_pacientes, fila_de_espera)) {
        printf("Dados salvos com sucesso!\n\n");
    }
    else {
        printf("ERRO: Não foi possível salvar os dados.\n\n");
    }

    // Limpar memória antes de sair
    fila_apagar(&fila_de_espera);
    lista_apagar(&lista_de_pacientes);

    printf("Sistema encerrado com sucesso.\n");
    return 0;
}