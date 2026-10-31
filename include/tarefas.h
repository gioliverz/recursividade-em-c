#ifndef TAREFAS_H
#define TAREFAS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Limite operacional estabelecido no escopo do projeto 
#define MAX_SUBTAREFAS 10

// Níveis de prioridade 
#define PRIORIDADE_BAIXA 1
#define PRIORIDADE_MEDIA 2
#define PRIORIDADE_ALTA  3

// Estrutura Data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

// Funções para manipulação de datas?? 
Data criar_data(int dia, int mes, int ano);
int validar_data(Data d);
Data parse_data(const char* str_prazo);
int comparar_datas(Data d1, Data d2);
void formatar_data(Data d, char* buffer, size_t tamanho);


// Estrutura para Subtarefas
typedef struct Subtarefa {
    int id;
    char descricao[100];
    int concluida; //valor booleano(0: não concluída, 1:concluída)
    int id_tarefa_pai;
    struct Subtarefa* proxima;// gi: Lista ligada de subtarefas???
} Subtarefa;

// Estrutura para Tarefa principal
typedef struct Tarefa {
    int id;
    char descricao[100];
    Data prazo;                  // tipo Data
    int prioridade;              // 1 - Alta, 2 - Média, 3 - Baixa
    float porcentagem_conclusao; // Calculada recursivamente/automaticamente
    Subtarefa* subtarefas;       // Ponteiro para a lista de subtarefas
    int qtd_subtarefas;          // Quantidade de subtarefas
    struct Tarefa* proxima;      // gi: Ponteiro para a próxima tarefa??
} Tarefa;

// --- Gestão Geral de Tarefas ---
Tarefa* criar_tarefa(int id, const char* descricao, const char* prazo, int prioridade);
int remover_tarefa(Tarefa** lista_geral, int id_tarefa);
int editar_tarefa(Tarefa* lista, int id_tarefa, const char* nova_descricao, const char* novo_prazo, int nova_prioridade);
void listar_tarefas(const Tarefa* lista);

// --- Gestão de Subtarefas ---
int criar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* descricao, int conclusao);
int remover_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa);
int editar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* nova_descricao, float nova_conclusao);
void listar_sub(const Tarefa* lista);

// --- Cálculo do Progresso e Atualização ---
float calculo_progresso(int id_tarefa_pai, const Tarefa* tarefa, const Subtarefa* sub);
void atualizaConclusao(Tarefa* tarefa, const Subtarefa* sub);

// --- FILTRO DE TAREFAS ---
Tarefa* busca_ID(Tarefa* lista, int id_tarefa);
void ordenacao_por_prioridade(Tarefa** lista);
void ordenarcao_por_prazo(Tarefa** lista_geral);

// --- Gestão de Memória Dinâmica ---
void liberar_subtarefas(Subtarefa* lista);
void liberar_todas_tarefas(Tarefa** lista);

#endif // TAREFAS_H