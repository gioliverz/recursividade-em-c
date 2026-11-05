#ifndef TAREFAS_H
#define TAREFAS_H

#pragma once
//para garantir que o arquivo seja incluído apenas uma vez
//durante a compilação

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Limite operacional estabelecido no escopo do projeto */
#define MAX_SUBTAREFAS 10

/* Níveis de prioridade */
#define PRIORIDADE_BAIXA 1
#define PRIORIDADE_MEDIA 2
#define PRIORIDADE_ALTA  3

/**
 * Estrutura auxiliar para manipulação segura e padronizada de prazos
 */
typedef struct {
    int dia; // 1 a 31
    int mes; // 1 a 12
    int ano; // 4 dígitos (ex: 2026)
} Data;

/**
 * Estrutura da menor unidade de ação (Subtarefa)
 * Organizada via lista simplesmente encadeada vinculada a uma Tarefa pai.
 */
typedef struct Subtarefa {
    int id;                    // Identificador sequencial no escopo da tarefa pai (1, 2, 3...)
    char descricao[101];       // Até 100 caracteres (+ 1 terminador '\0')
    int concluida;             // 0 para Pendente, 1 para Concluída
    int id_tarefa_pai;         // Chave estrangeira ligando à Tarefa mãe
    struct Subtarefa* proxima; // Ponteiro para o próximo nó da lista de subtarefas
} Subtarefa;

/**
 * Estrutura da entidade principal (Tarefa)
 * Organizada via lista simplesmente encadeada no escopo global.
 */
typedef struct Tarefa {
    int id;                     // Identificador único global sequencial
    char descricao[101];        // Até 100 caracteres (+ 1 terminador '\0')
    Data prazo;                 // Estrutura de data limite
    int prioridade;             // 1 (Baixa), 2 (Média), 3 (Alta)
    float percentual_conclusao; // 0.0% a 100.0% calculado recursivamente
    int qtd_subtarefas;         // Quantidade atual de subtarefas (máximo MAX_SUBTAREFAS)
    Subtarefa* subtarefas;      // Ponteiro para o início da lista ligada de subtarefas
    struct Tarefa* proxima;     // Ponteiro para a próxima tarefa na lista global
} Tarefa;

/* Funções auxiliares de datas */
Data criar_data(int dia, int mes, int ano);
int validar_data(Data d);
Data parse_data(const char* str_prazo);
int comparar_datas(Data d1, Data d2);
void formatar_data(Data d, char* buffer, size_t tamanho);

/* Manipulação de Tarefas */
Tarefa* criar_tarefa(int id, const char* descricao, const char* prazo, int prioridade);
int remover_tarefa(Tarefa** lista_geral, int id_tarefa);
int editar_tarefa(Tarefa* lista, int id_tarefa, const char* nova_descricao, const char* novo_prazo, int nova_prioridade);
void listar_tarefas(const Tarefa* lista);

/* Manipulação de Subtarefas */
int criar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* descricao, int conclusao);
int remover_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa);
int editar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* nova_descricao, float nova_conclusao);
void listar_sub(const Tarefa* lista);

/* Filtros e Busca */
Tarefa* busca_ID(Tarefa* lista, int id_tarefa);
void ordenacao_por_prioridade(Tarefa** lista);
void ordenarcao_por_prazo(Tarefa** lista_geral);

/* Mecanismo Recursivo de Cálculo e Atualização */
float calculo_progresso(int id_tarefa_pai, const Tarefa* tarefa, const Subtarefa* sub);
void atualizaConclusao(Tarefa* tarefa, const Subtarefa* sub);

/* Gestão de Memória Dinâmica */
void liberar_subtarefas(Subtarefa* lista);
void liberar_todas_tarefas(Tarefa** lista);

#endif /* TAREFAS_H */
