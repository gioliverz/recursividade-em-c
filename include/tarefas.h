#ifndef TAREFAS_H
#define TAREFAS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Estrutura Data
typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

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
    int prioridade;              // Ex: 1 - Alta, 2 - Média, 3 - Baixa
    float porcentagem_conclusao; // Calculada recursivamente/automaticamente
    Subtarefa* subtarefas;       // Ponteiro para a lista de subtarefas
    int qtd_subtarefas;          // Quantidade de subtarefas
    struct Tarefa* proxima;      // gi: Ponteiro para a próxima tarefa??
} Tarefa;

// --- Gestão Geral de Tarefas (CRUD) ---
Tarefa* criar_tarefa();
// read
// update
// delete

// --- Gestão de Subtarefas ---


// --- cálculo do progresso e atualização ---
float calculo_progresso (int id_tarefa_pai, const Tarefa* tarefa, const Subtarefa* sub);
void atualizaConclusao (Tarefa* tarefa, const Subtarefa* sub);

// --- FILTRO DE TAREFAS ---
Tarefa* busca_ID (Tarefa* lista_geral, int id_tarefa);
void ordenacao_por_prioridade (Tarefa** lista_geral); 
void ordenarcao_por_prazo (Tarefa** lista_geral); 



#endif // TAREFAS_H