#ifndef MENU_H
#define MENU_H

#include "tarefas.h"

// MENU PRINCIPAL
void exibir_menu_principal(void);
void executar_opcao(int opcao);
void exibir_menu_tarefas(void);
void exibir_filtro(void); //gi: que que vcs acham de add isso?

// MENU DE TAREFAS
void menu_cadastrar_tarefa (Tarefa** lista_geral);




// MENU DE FILTRO
void menu_filtrar_tarefas(Tarefa** lista_geral); // gi: FUNÇÃO NOVA?




#endif // MENU_H