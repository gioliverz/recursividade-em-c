#include "../include/tarefas.h"     // Se usar apenas "tarefas.c", o compilador procuraria na mesma pasta (src) pelo arquivo .h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Na linha 229 preciso da opinião de vocês sobre a estrutura de uma das funções

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

// Remover uma tarefa da lista pelo id e liberar memória
int remover_tarefa(Tarefa** lista_geral, int id){
    Tarefa* atual = *lista_geral;
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
        *lista_geral = atual->proxima; // raiz passa a ser a segunda tarefa
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

// Listar todas as tarefas da lista encadeada
void listar_tarefas(const Tarefa* lista){
    const Tarefa* atual = lista;
    if (atual == NULL) {
        printf("Nenhuma tarefa cadastrada.\n");
        return;
    }
    printf("--- Lista de Tarefas Principais ---\n");
    while (atual != NULL) {
        printf("ID: %d | Descrição: %s | Prazo: %02d/%02d/%d | Progresso: %.1f%%\n", atual->id,atual->descricao,atual->prazo,atual->porcentagem_conclusao);
        atual = atual->proxima;
    }
}

// -- Gestão de subtarefas --

// Adicionar uma subtarefa a uma tarefa existente
int criar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* descricao, int conclusao){
    // procurar a tarefa pai
    Tarefa* pai = buscar_tarefa_por_id(lista, id_tarefa_pai);
    if (pai == NULL) {
        return 0; // tarefa pai não foi encontrada
    }
    // validar o limite de subtarefas
    if (pai->qtd_subtarefas >= 10) {
        printf("Aviso: Limite maximo de 10 subtarefas atingido para esta tarefa.\n");
        return 0;
    }
    // inserir a nova subtarefa na posição correta
    int indice = pai->qtd_subtarefas;
    pai->subtarefas[indice].id = indice + 1; // id sequencial local
    strncpy(pai->subtarefas[indice].descricao, descricao, 99);
    pai->subtarefas[indice].descricao[99] = '\0';
    pai->subtarefas[indice].concluida = 0; // tarefa está pendente
    pai->subtarefas[indice].id_tarefa_pai = id_tarefa_pai;
    // incrementar o contador de subtarefas
    pai->qtd_subtarefas++;
    // atualizar o progresso usando a função recursiva
    pai->porcentagem_conclusao = calcular_progressao(pai,0,0);
    
    return 1; // subtarefa adicionada com sucesso
}

// Remover uma subtarefa
int remover_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa){
    Tarefa* pai = buscar_tarefa_por_id(lista, id_tarefa_pai);
    if (pai == NULL)
        return 0; // tarefa pai não encontrada
    int encontrada = -1;
    // procurar o índice da subtarefa pelo id
    for (int i = 0; i < pai->qtd_subtarefas; i++) {
        if (pai->subtarefas[i].id == id_subtarefa) {
            encontrada = i;
            break;
        }
    }
    if (encontrada == -1)
        return 0; // subtarefa não encontrada
    // reorganizar as subtarefas
    for (int i = encontrada; i < pai->qtd_subtarefas - 1; i++) {
        pai->subtarefas[i] = pai->subtarefas[i+1];
    }
    // reduzir a quantidade de subtarefas
    pai->qtd_subtarefas--;
    // atualizar o progresso
    pai->porcentagem_conclusao = calcular_progressao(pai,0,0);

    return 1; // subtarefa removida com sucesso
}

// Alterar dados de uma subtarefa e atualizar o progresso
int editar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* nova_descricao, int nova_conclusao){
    Tarefa* pai = buscar_tarefa_por_id(lista, id_tarefa_pai);
    if (pai == NULL)
        return 0;
    
    for (int i = 0; i< pai->qtd_subtarefas; i++) {
        if (pai->subtarefas[i].id == id_subtarefa) {
            strncpy(pai->subtarefas[i].descricao, nova_descricao, 99);
            pai->subtarefas[i].descricao[99] = '\0';
            pai->subtarefas[i].concluida = (nova_conclusao == 1) ? 1 : 0; // garantir valor booleano
            pai->porcentagem_conclusao = calcular_progressao(pai,0,0);

            return 1; // subtarefa editada com sucesso
        }
    }
    return 0; // subtarefa não encontrada
}

void listar_sub(const Tarefa* lista){
    if (lista == NULL)
        return;
    if (lista->qtd_subtarefas == 0) {
        printf("Esta tarefa nao possui subtarefas.\n");
        return;
    }
    printf("--- Subtarefas da Tarefa ID %d ---\n", lista->id);
    for (int i = 0; i < lista->qtd_subtarefas; i++) {
        printf(" ID Subtarefa: %d | Descricao: %s | Status: %s\n",lista->subtarefas[i].id,lista->subtarefas[i].descricao,lista->subtarefas[i].concluida == 1 ? "Concluida" : "Pendente");
    }
}

// --- Calculo do progresso e atualização ---

// Algoritmo recursivo para calcular o percentual de conclusão das subtarefas
float calcular_progressao(Tarefa* tarefa, int indice, int concluidas){     // alterei a estrutura dos parâmetros, vejam se concordam !!
    // caso base: a tarefa não tem subtarefas
    if(tarefa->qtd_subtarefas == 0)
        return 0.0f; // sem subtarefas, progresso é 0%
    if (indice == tarefa->qtd_subtarefas) {     // caso base de parada (fim do vetor)
        return ((float)concluidas / tarefa-> qtd_subtarefas) * 100.0f; // conversão de tipo
    }
    // caso recursivo: se a subtarefa atual estiver concluída, soma 1
    if (tarefa->subtarefas[indice].concluida == 1) {
        concluidas++;
    }
    // avança para o próximo índice
    return calcular_progressao(tarefa, indice + 1, concluidas);
}

void atualizaConclusao(Tarefa* tarefa, const Subtarefa* sub){
    if (tarefa == NULL)
        return;
    tarefa->porcentagem_conclusao = calcular_progressao(tarefa,0,0);
}

// --- Filtro de tarefas ---

// Localizar uma tarefa na lista encadeada pelo ID
Tarefa* buscar_tarefa_por_id(Tarefa* lista_geral, int id){
    Tarefa* atual = lista_geral;
    // percorrer a lista
    while (atual != NULL) {
        if (atual->id == id) {
            return atual; // encontrou a tarefa correta
        }
        atual = atual->proxima; // avança para a próxima tarefa da lista
    }
    return NULL; // percorreu tudo e não achou
}

// Função auxiliar para inverter o conteúdo de duas tarefas
void trocar_dados_tarefas(Tarefa* a, Tarefa* b){
    int aux_id = a->id;
    a->id = b->id;
    b->id = aux_id;

    float temp_prog = a->porcentagem_conclusao;
    a->porcentagem_conclusao = b->porcentagem_conclusao;
    b->porcentagem_conclusao = temp_prog;

    int aux_qtd = a->qtd_subtarefas;
    a->qtd_subtarefas = b->qtd_subtarefas;
    b->qtd_subtarefas = aux_qtd;

    int aux_prioridade = a->prioridade;
    a->prioridade = b->prioridade;
    b->prioridade = aux_prioridade;

    Data temp_prazo = a->prazo;
    a->prazo = b->prazo;
    b->prazo = temp_prazo;

    char temp_desc[100];
    strcpy(temp_desc, a->descricao);
    strcpy(a->descricao, b->descricao);
    strcpy(b->descricao, temp_desc);

    for (int i = 0; i < 10; i++) {
        Subtarefa temp_sub = a->subtarefas[i];
        a->subtarefas[i] = b->subtarefas[i];
        b->subtarefas[i] = temp_sub;
    }
}

void ordenacao_por_prioridade(Tarefa** lista){
    if (lista == NULL || *lista == NULL)
        return;
    int trocou;
    Tarefa* atual;
    Tarefa* ultimo_verificado = NULL;
    do {
        trocou = 0;
        atual = *lista;
        while (atual->proxima != ultimo_verificado) {
            if (atual->prioridade < atual->proxima->prioridade) {
                trocar_dados_tarefas(atual, atual->proxima);
                trocou = 1;
            }
            atual = atual->proxima;
        }
        ultimo_verificado = atual;
    } while (trocou);
}

void ordenacao_por_prazo(Tarefa** lista_geral){
    if (lista_geral = NULL || *lista_geral == NULL)
        return;
    int trocou;
    Tarefa* atual;
    Tarefa* ultimo_verificado = NULL;
    do {
        trocou = 0;
        atual = *lista_geral;
        while (atual->proxima != ultimo_verificado) {
            if(comparar_datas(atual->prazo,atual->proxima->prazo) > 0) {
                trocar_dados_tarefas(atual,atual->proxima);
                trocou = 1;
            }
            atual = atual->proxima;
        }
        ultimo_verificado = atual;
    } while (trocou);
}

