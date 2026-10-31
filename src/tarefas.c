#include "../include/tarefas.h"     // Se usar apenas "tarefas.c", o compilador procuraria na mesma pasta (src) pelo arquivo .h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -- Funções para manipulação de datas --
Data criar_data(int dia, int mes, int ano){
    Data d;
    d.dia = dia;
    d.mes = mes;
    d.ano = ano;
    return d;
}

// Validar se a data existe no calendário
int validar_data(Data d){
    if(d.ano < 2026 || d.mes < 1 || d.mes >12 || d.dia < 1) {
        return 0; // data inválida
    }
    int ultimos_dias_mes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    // verificar ano bissexto
    if ((d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0)) {
        ultimos_dias_mes[1] = 29;
    }
    if (d.dia > ultimos_dias_mes[d.mes - 1]) {
        return 0; // data inválida
    }
    return 1; // data válida
}

// Converter uma string DD/MM/AAAA para uma estrutura Data
Data parse_data(const char* str_prazo){
    Data d = {0,0,0};
    if (str_prazo == NULL)
        return d; // avalia se a string está vazia
    sscanf(str_prazo, "%d/%d/%d",&d.dia,&d.mes,&d.ano); // extrai dados de dentro de uma string que já existe (no caso, str_prazo)
    return d;
}

// Comparar duas datas: retorn -1 se d1 for menor, 1 se d2 for maior e 0 se forem iguais
int comparar_datas(Data d1, Data d2){
    if(d1.ano != d2.ano)
        return (d1.ano < d2.ano) ? -1 : 1;
    if (d1.mes != d2.mes)
        return (d1.mes < d2.mes) ? -1 : 1;
    if (d1.dia != d2.dia)
        return (d1.dia < d2.dia) ? -1 : 1;
    return 0;
}

// Formatar a struct Data de volta para a forma de string
void formatar_data(Data d, char* buffer, size_t tamanho){
    snprintf(buffer, tamanho, "%02d/%02d/%d", d.dia, d.mes, d.ano); // joga o resultado formatado dentro de uma string (o n no nome significa "número limite de caracteres")
}

// -- Gestão geral de tarefas --

Tarefa* criar_tarefa(int id, const char* descricao, const char* prazo, int prioridade){
    Tarefa* nova_tarefa = (Tarefa*)malloc(sizeof(Tarefa));
    // validar se o computador tem memória suficiente
    if (nova_tarefa == NULL) {
        return NULL;
    }
    nova_tarefa->id = id;
    strncpy(nova_tarefa->descricao, descricao, 99);
    nova_tarefa->descricao[99] = '\0'; // garantir caractere nulo no final
    nova_tarefa->prazo = parse_data(prazo);
    // validar prioridade
    if (prioridade >= 1 && prioridade <= 3) {
        nova_tarefa->prioridade = prioridade;
    } else {
        nova_tarefa->prioridade = 1; // atribuição padrão
    }
    // inicializar os campos de controle e subtarefas
    nova_tarefa->porcentagem_conclusao = 0.0f;
    nova_tarefa->qtd_subtarefas = 0;
    nova_tarefa->proxima = NULL; // começa apontando para o vazio

    return nova_tarefa;
}

// Localizar uma tarefa na lista encadeada pelo ID
Tarefa* buscar_tarefa_por_id(Tarefa* raiz_ou_lista, int id){
    Tarefa* atual = raiz_ou_lista;
    // percorrer a lista
    while (atual != NULL) {
        if (atual->id == id) {
            return atual; // encontrou a tarefa correta
        }
        atual = atual->proxima; // avança para a próxima tarefa da lista
    }
    return NULL; // percorreu tudo e não achou
}

// Remover uma tarefa da lista pelo id e liberar memória
int remover_tarefa(Tarefa** raiz_ou_lista, int id){
    Tarefa* atual = *raiz_ou_lista;
    Tarefa* anterior = NULL;
    
    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->proxima;
    }
    //se não encontrou a tarefa
    if (atual == NULL) {
        return 0;
    }
    // a tarefa a ser removida é a primeira da lista
    if (anterior == NULL) {
        *raiz_ou_lista = atual->proxima; // raiz passa a ser a segunda tarefa
    } else {
        anterior->proxima = atual->proxima; // contorna o elemento removido
    }

    free(atual);
    return 1; // removida com sucesso
}

// Atualizar os dados de uma tarefa existente
int editar_tarefa(Tarefa* tarefa, const char* nova_descricao, const char* novo_prazo, int nova_prioridade){
    if (tarefa == NULL)
        return 0; // tarefa inexistente
    // atualizar descrição
    strncpy(tarefa->descricao, nova_descricao, 99);
    tarefa->descricao[99] = '\0';
    // atualizar o prazo convertendo a string
    tarefa->prazo = parse_data(novo_prazo);
    // validar e atualizar a prioridade
    if (nova_prioridade >= 1 && nova_prioridade <= 3) {
        tarefa->prioridade = nova_prioridade;
    } else {
        tarefa->prioridade = 1;
    }
    return 1; // atualização bem-sucedida
}

// -- Gestão de subtarefas --

