/**
 * ============================================================================
 * PROJETO PRÁTICO INTEGRADOR — TEMA T5: Gerenciador de Tarefas com Subtarefas
 * Diretório: recursividade-em-c/src/main.c
 * Ponto de Entrada da Aplicação e Loop Principal de Execução
 * ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>

#ifdef __has_include
  #if __has_include("../include/tarefas.h")
    #include "../include/tarefas.h"
    #include "../include/arquivos.h"
    #include "../include/menu.h"
  #else
    #include "tarefas.h"
    #include "arquivos.h"
    #include "menu.h"
  #endif
#else
  #include "../include/tarefas.h"
  #include "../include/arquivos.h"
  #include "../include/menu.h"
#endif

/* Inicializa o sistema com dados demonstrativos caso não exista arquivo salvo */
static void semear_dados_exemplo(Tarefa** lista) {
    if (lista == NULL || *lista != NULL) return;

    printf("\n[SISTEMA] Populando dados iniciais de demonstracao...\n");

    // Tarefa 1: Projeto Integrador de C
    Tarefa* t1 = criar_tarefa(1, "Projeto Integrador de C", "15/11/2026", PRIORIDADE_ALTA);
    *lista = t1;
    criar_sub(*lista, 1, 1, "Modelar structs de Data, Tarefa e Subtarefa", 1);
    criar_sub(*lista, 1, 2, "Implementar calculo recursivo de progresso", 1);
    criar_sub(*lista, 1, 3, "Implementar persistencia binaria segura", 1);
    criar_sub(*lista, 1, 4, "Elaborar relatorio de justificativas tecnicas", 0);

    // Tarefa 2: Estudo para Prova de Calculo
    Tarefa* t2 = criar_tarefa(2, "Estudo para Prova de Calculo", "10/11/2026", PRIORIDADE_MEDIA);
    t1->proxima = t2;
    criar_sub(*lista, 2, 1, "Revisar Integrais por Partes", 1);
    criar_sub(*lista, 2, 2, "Resolver lista de exercicios capitulo 7", 0);

    // Tarefa 3: Manutencao do Computador
    Tarefa* t3 = criar_tarefa(3, "Manutencao e Limpeza do PC", "20/11/2026", PRIORIDADE_BAIXA);
    t2->proxima = t3;
    criar_sub(*lista, 3, 1, "Fazer backup dos arquivos na nuvem", 1);

    printf("[SISTEMA] 3 tarefas de exemplo inseridas (com subtarefas)\n\n");
}

int main(void) {
    Tarefa* lista_geral = NULL;

    printf("====================================================================\n");
    printf("              GERENCIADOR DE TAREFAS HIERARQUICO (C)        \n");
    printf("            Disciplina: Algoritmos e Programacao em C               \n");
    printf("====================================================================\n");

    // Verifica se já existe base binária salva em disco
    if (verificar_existencia_arquivo("dados.bin")) {
        printf("\nArquivo de persistencia 'dados.bin' detectado!\n");
        printf("Deseja carregar os dados salvos? (1 - Sim, 0 - Nao / Novos): ");
        char buf[16];
        if (fgets(buf, sizeof(buf), stdin) && (buf[0] == '1' || buf[0] == 's' || buf[0] == 'S')) {
            lista_geral = carregar_tarefas_binario("dados.bin");
            if (lista_geral != NULL) {
                printf("Base de tarefas carregada com sucesso!\n");
            } else {
                printf("Falha ao ler dados.bin. Iniciando com dados de exemplo.\n");
                semear_dados_exemplo(&lista_geral);
            }
        } else {
            semear_dados_exemplo(&lista_geral);
        }
    } else {
        semear_dados_exemplo(&lista_geral);
    }

    int opcao = -1;
    do {
        exibir_menu_principal();
        opcao = ler_inteiro("Selecione uma opcao: ");

        if (opcao != 0) {
            executar_opcao_principal(&lista_geral, opcao);
            pausar_tela();
            limpar_tela();
        }
    } while (opcao != 0);

    // Liberação preventiva e total de memória alocada na heap
    printf("\nLiberando memoria alocada para as listas encadeadas...\n");
    liberar_todas_tarefas(&lista_geral);
    printf("Memoria liberada com sucesso. Programa finalizado.\n");

    return 0;
}
