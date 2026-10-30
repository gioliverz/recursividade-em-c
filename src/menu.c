#include "menu.h"

void executar_opcao_principal(int opcao) {
    switch (opcao) {
        case 1:
            exibir_menu_tarefas();
            break;
        case 2:
            exibir_filtro();
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
    scanf("%d", &opcao); // antes de executar_opcao_principal(opcao),
    // confere se a opção é válida antes de chamar a função:
    if (opcao == 1 || opcao == 2) {
        executar_opcao_principal(opcao);
    } else {
        printf("Opção inválida. Por favor, tente novamente.\n");
    }
}


