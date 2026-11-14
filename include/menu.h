/**
 * ============================================================================
 * PROJETO PRÁTICO INTEGRADOR — TEMA T5: Gerenciador de Tarefas com Subtarefas
 * Diretório: recursividade-em-c/include/menu.h
 * Módulo 1: Interface de Usuário e Validações via Terminal
 * ============================================================================
 */

#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefas.h"
#include "arquivos.h"

/* Navegação e Menus Principais */
int exibir_menu_principal(void);
void exibir_menu_tarefas(Tarefa** lista_geral);
void exibir_menu_subtarefas(Tarefa** lista_geral);
void exibir_filtro(Tarefa** lista_geral);
void executar_opcao_principal(Tarefa** lista_geral, int opcao);

/* Formulários de Entrada de Dados */
void menu_cadastrar_tarefa(Tarefa** lista_geral);
void menu_adicionar_subtarefa(Tarefa** lista_geral);
void menu_atualizar_subtarefa(Tarefa** lista);
void menu_exportar_agenda(Tarefa* lista);

/* Exibição e Formatação de Dados */
const char* obter_texto_prioridade(int prioridade);
void mostrar_lista_tarefas(const Tarefa* lista);
void mostrar_detalhes_tarefa(const Tarefa* lista);
void exibir_mensagem(const char* mensagem);

/* Gerenciamento da Interface */
void limpar_tela(void);
void pausar_tela(void);
void exibir_mensagem(const char* mensagem);

#endif /* MENU_H */
