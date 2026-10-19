#include "menu.h"

void executar_opcao(int opcao) {
    switch (opcao) {
        case 1:
            exibir_menu_tarefas();
            break;
        case 2:
            exibir_filtro();
            break;
        default:
            printf("Opção inválida. Por favor, tente novamente.\n");
            break;
    }
}

void exibir_menu_principal(void) {
    printf("=== Gerenciador de Tarefas ===\n");
    printf("- Gerenciar Tarefas \n");
    printf("- Filtrar Tarefas \n");
    printf("Digite 1 para Gerenciar Tarefas\n");
    printf("Digite 2 para Filtrar Tarefas\n");
    int opcao;
    scanf("%d", &opcao);
    executar_opcao(opcao);
}


