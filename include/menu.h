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
void exibir_menu_principal(void);
void exibir_menu_tarefas(void);
void exibir_menu_subtarefas(void);
void exibir_filtro(void);
void executar_opcao_principal(Tarefa** lista_geral, int opcao);

/* Formulários de Entrada de Dados */
void menu_cadastrar_tarefa(Tarefa** lista_geral);
void menu_adicionar_subtarefa(Tarefa* lista);
void menu_atualizar_progresso_subtarefa(Tarefa* lista);
void menu_exportar_agenda(Tarefa* lista);

/* Exibição e Formatação de Dados */
void mostrar_lista_tarefas(const Tarefa* lista);
void mostrar_detalhes_tarefa(Tarefa** lista_global, int id);
void exibir_mensagem(const char* mensagem);

/* Gerenciamento da Interface */
void limpar_tela(void);
void pausar_tela(void);

/* Leitura e Validação de Entrada de Teclado */
void ler_string_segura(char* buffer, int tamanho);
int ler_inteiro(const char* rotulo);

#endif /* MENU_H */
