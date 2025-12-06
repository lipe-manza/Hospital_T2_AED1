#include <stdio.h>
#include "avl.h"
#include "heap.h"
#include "op.h"

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
    printf("7. Sair\n");
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

    HS* fila_de_espera = fila_criar(); // Cria a fila da triagem
    AVL* lista_de_pacientes = lista_criar(); // Cria a lista de pacientes

    // Carregar dados salvos
    printf("===========================================\n");
    printf("   Sistema de Gestão Hospitalar - PS\n");
    printf("===========================================\n\n");
    printf("Carregando dados salvos...\n");
    //falta implementar a funcao de carregar json
    printf("\n");


    // Inicio do menu e dos switchs
    do {
        choice = display_menu();
        opcoes(lista_de_pacientes, fila_de_espera, choice);

    } while (choice != 5);

    // Salvar dados antes de sair
    printf("\nSalvando dados...\n");
    // falta implementar a funcao de salvar json
    printf("\nDados salvos com sucesso!\n\n");

    // Limpar memória antes de sair
    fila_apagar(&fila_de_espera);
    lista_apagar(&lista_de_pacientes);

    printf("Sistema encerrado com sucesso.\n");
    return 0;
}