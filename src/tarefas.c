#include "../include/tarefas.h"

// FUNÇÕES AUXILIARES DE DATAS:
Data criar_data(int dia, int mes, int ano) {
    Data d;
    d.dia = dia;
    d.mes = mes;
    d.ano = ano;
    return d;
}

int validar_data(Data d) {
    if (d.ano < 1900 || d.ano > 2100) return 0;
    if (d.mes < 1 || d.mes > 12) return 0;
    
    int dias_mes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // último dia do mês de fevereiro depende se o ano é bissexto

    // Ano bissexto
    int bissexto = (d.ano % 4 == 0 && d.ano % 100 != 0) || (d.ano % 400 == 0);
    if (bissexto) {
        dias_mes[2] = 29;
    }
    
    if (d.dia < 1 || d.dia > dias_mes[d.mes]) return 0;
    return 1;
}

int parse_data(const char* str_prazo, Data* resultado) {
    if (str_prazo == NULL || resultado == NULL) return 0;

    int dia = 0, mes = 0, ano = 0;
    char extra;
    int lidos = sscanf(str_prazo, "%d/%d/%d %c", &dia, &mes, &ano, &extra);
    if (lidos != 3) {
        lidos = sscanf(str_prazo, "%d-%d-%d %c", &dia, &mes, &ano, &extra);
    }
    if (lidos != 3) return 0;

    Data temp = {dia, mes, ano};
    if (!validar_data(temp)) return 0;

    *resultado = temp;
    return 1;
}

int comparar_datas(Data d1, Data d2) {
    if (d1.ano != d2.ano) return d1.ano - d2.ano;
    if (d1.mes != d2.mes) return d1.mes - d2.mes;
    return d1.dia - d2.dia;
}

void formatar_data(Data d, char* buffer, size_t tamanho) {
    if (buffer == NULL || tamanho == 0) return;
    snprintf(buffer, tamanho, "%02d/%02d/%04d", d.dia, d.mes, d.ano);
}

/* ============================================================================
 * MANIPULAÇÃO DE TAREFAS PRINCIPAIS
 * ============================================================================ */

Tarefa* criar_tarefa(int id, const char* descricao, const char* prazo, int prioridade) {
    Tarefa* nova = (Tarefa*)malloc(sizeof(Tarefa));
    if (nova == NULL) {
        fprintf(stderr, "Erro de alocação de memória para nova Tarefa!\n");
        return NULL;
    }

    nova->id = id;
    
    if (descricao != NULL) {
        strncpy(nova->descricao, descricao, sizeof(nova->descricao) - 1);
        nova->descricao[sizeof(nova->descricao) - 1] = '\0';
    } else {
        strcpy(nova->descricao, "Sem descrição");
    }

    if (!parse_data(prazo, &nova->prazo)) {
        fprintf(stderr, "Erro: prazo inválido. Use DD/MM/AAAA ou DD-MM-AAAA.\n");
        free(nova);
        return NULL;
    }

    if (prioridade < PRIORIDADE_BAIXA || prioridade > PRIORIDADE_ALTA) {
        nova->prioridade = PRIORIDADE_BAIXA; // Padrão conforme escopo
    } else {
        nova->prioridade = prioridade;
    }

    nova->porcentagem_conclusao = 0.0f;
    nova->qtd_subtarefas = 0;
    nova->subtarefas = NULL;
    nova->proxima = NULL;

    return nova;
}

int remover_tarefa(Tarefa** lista_geral, int id_tarefa) {
    if (lista_geral == NULL || *lista_geral == NULL) {
        return 0; // Lista vazia
    }

    Tarefa* atual = *lista_geral;
    Tarefa* anterior = NULL;

    while (atual != NULL && atual->id != id_tarefa) {
        anterior = atual;
        atual = atual->proxima;
    }

    if (atual == NULL) {
        return 0; // Tarefa com ID especificado não encontrada
    }

    // Desconecta da lista principal
    if (anterior == NULL) {
        *lista_geral = atual->proxima;
    } else {
        anterior->proxima = atual->proxima;
    }

    // Libera recursivamente toda a lista encadeada de subtarefas da tarefa
    liberar_subtarefas(atual->subtarefas);
    free(atual);

    return 1; // Removida com sucesso
}

int editar_tarefa(Tarefa* lista, int id_tarefa, const char* nova_descricao, const char* novo_prazo, int nova_prioridade) {
    Tarefa* t = busca_ID(lista, id_tarefa);
    if (t == NULL) {
        return 0;
    }

    Data prazo;
    int atualizar_prazo = novo_prazo != NULL && strlen(novo_prazo) > 0;
    if (atualizar_prazo && !parse_data(novo_prazo, &prazo)) {
        return 0;
    }

    if (nova_descricao != NULL && strlen(nova_descricao) > 0) {
        strncpy(t->descricao, nova_descricao, sizeof(t->descricao) - 1);
        t->descricao[sizeof(t->descricao) - 1] = '\0';
    }

    if (atualizar_prazo) t->prazo = prazo;

    if (nova_prioridade >= PRIORIDADE_BAIXA && nova_prioridade <= PRIORIDADE_ALTA) {
        t->prioridade = nova_prioridade;
    }

    return 1;
}

void listar_tarefas(const Tarefa* lista) {
    if (lista == NULL) {
        printf("\nNenhuma tarefa cadastrada no sistema.\n");
        return;
    }

    const Tarefa* t = lista;
    char data_str[16];

    printf("\n========================================================================================\n");
    printf("%-4s | %-30s | %-10s | %-10s | %-9s | %-10s\n", 
           "ID", "DESCRICAO", "PRAZO", "PRIORIDADE", "PROGRESSO", "SUBTAREFAS");
    printf("----------------------------------------------------------------------------------------\n");

    while (t != NULL) {
        formatar_data(t->prazo, data_str, sizeof(data_str));
        const char* prio_txt = (t->prioridade == PRIORIDADE_ALTA) ? "Alta" :
                               (t->prioridade == PRIORIDADE_MEDIA) ? "Media" : "Baixa";

        printf("%-4d | %-30.30s | %-10s | %-10s | %6.1f%%    | %d/%d\n",
               t->id, t->descricao, data_str, prio_txt, t->porcentagem_conclusao,
               t->qtd_subtarefas, MAX_SUBTAREFAS);
        
        t = t->proxima;
    }
    printf("========================================================================================\n");
}

/* ============================================================================
 * MANIPULAÇÃO DE SUBTAREFAS
 * ============================================================================ */

int criar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* descricao, int conclusao) {
    Tarefa* pai = busca_ID(lista, id_tarefa_pai);
    if (pai == NULL) {
        return 0; // Tarefa mãe não encontrada
    }

    // Regra do escopo: limite máximo de 10 subtarefas
    if (pai->qtd_subtarefas >= MAX_SUBTAREFAS) {
        return -1; // Limite excedido
    }

    Subtarefa* nova = (Subtarefa*)malloc(sizeof(Subtarefa));
    if (nova == NULL) {
        return 0;
    }

    nova->id = id_subtarefa;
    nova->id_tarefa_pai = id_tarefa_pai;
    nova->concluida = (conclusao == 1) ? 1 : 0; // Padrão 0 conforme escopo
    
    if (descricao != NULL) {
        strncpy(nova->descricao, descricao, sizeof(nova->descricao) - 1);
        nova->descricao[sizeof(nova->descricao) - 1] = '\0';
    } else {
        strcpy(nova->descricao, "Subtarefa");
    }

    nova->proxima = NULL;

    // Inserção no final da lista encadeada de subtarefas para manter a ordem sequencial
    if (pai->subtarefas == NULL) {
        pai->subtarefas = nova;
    } else {
        Subtarefa* aux = pai->subtarefas;
        while (aux->proxima != NULL) {
            aux = aux->proxima;
        }
        aux->proxima = nova;
    }

    pai->qtd_subtarefas++;

    // Gatilho obrigatório: recalcular progresso recursivamente
    atualizaConclusao(pai, pai->subtarefas);

    return 1;
}

int remover_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa) {
    Tarefa* pai = busca_ID(lista, id_tarefa_pai);
    if (pai == NULL || pai->subtarefas == NULL) {
        return 0;
    }

    Subtarefa* atual = pai->subtarefas;
    Subtarefa* anterior = NULL;

    while (atual != NULL && atual->id != id_subtarefa) {
        anterior = atual;
        atual = atual->proxima;
    }

    if (atual == NULL) {
        return 0; // Subtarefa não encontrada
    }

    if (anterior == NULL) {
        pai->subtarefas = atual->proxima;
    } else {
        anterior->proxima = atual->proxima;
    }

    free(atual);
    pai->qtd_subtarefas--;

    // Recalcula progresso após a remoção
    atualizaConclusao(pai, pai->subtarefas);

    return 1;
}

int editar_sub(Tarefa* lista, int id_tarefa_pai, int id_subtarefa, const char* nova_descricao, float nova_conclusao) {
    Tarefa* pai = busca_ID(lista, id_tarefa_pai);
    if (pai == NULL || pai->subtarefas == NULL) {
        return 0;
    }

    Subtarefa* s = pai->subtarefas;
    while (s != NULL && s->id != id_subtarefa) {
        s = s->proxima;
    }

    if (s == NULL) {
        return 0;
    }

    if (nova_descricao != NULL && strlen(nova_descricao) > 0) {
        strncpy(s->descricao, nova_descricao, sizeof(s->descricao) - 1);
        s->descricao[sizeof(s->descricao) - 1] = '\0';
    }

    // Conclusão: aceita 1/0 ou valores >= 0.5 como 1
    if (nova_conclusao >= 0.0f) {
        s->concluida = (nova_conclusao >= 0.5f) ? 1 : 0;
    }

    // Gatilho obrigatório de recálculo recursivo
    atualizaConclusao(pai, pai->subtarefas);
    return 1;
}

void listar_sub(const Tarefa* lista) {
    if (lista == NULL) {
        printf("\nNenhuma tarefa para listar subtarefas.\n");
        return;
    }

    printf("\n=== SUBTAREFAS DA TAREFA #%d: '%s' ===\n", lista->id, lista->descricao);
    if (lista->subtarefas == NULL) {
        printf("  (Esta tarefa nao possui subtarefas cadastradas)\n");
        return;
    }

    const Subtarefa* s = lista->subtarefas;
    while (s != NULL) {
        printf("  [%s] Subtarefa #%d.%d: %s\n",
               s->concluida ? "X" : " ",
               s->id_tarefa_pai, s->id, s->descricao);
        s = s->proxima;
    }
}

/* ============================================================================
 * FILTROS E BUSCA
 * ============================================================================ */

Tarefa* busca_ID(Tarefa* lista, int id_tarefa) {
    Tarefa* atual = lista;
    while (atual != NULL) {
        if (atual->id == id_tarefa) {
            return atual;
        }
        atual = atual->proxima;
    }
    return NULL;
}

void ordenacao_por_prioridade(Tarefa** lista) {
    if (lista == NULL || *lista == NULL || (*lista)->proxima == NULL) {
        return; // Lista vazia ou com 1 elemento
    }

    int trocou;
    Tarefa* atual;
    Tarefa* ultimo_verificado = NULL;

    do {
        trocou = 0;
        atual = *lista;

        while (atual->proxima != ultimo_verificado) {
            // Ordena decrescente: Alta (3) > Media (2) > Baixa (1)
            if (atual->prioridade < atual->proxima->prioridade) {
                // Troca os dados do payload entre os nós para manter ponteiros estáveis
                Tarefa temp;
                
                // Copia temporária dos campos de dados
                temp.id = atual->id;
                memcpy(temp.descricao, atual->descricao, sizeof(temp.descricao));
                temp.prazo = atual->prazo;
                temp.prioridade = atual->prioridade;
                temp.porcentagem_conclusao = atual->porcentagem_conclusao;
                temp.qtd_subtarefas = atual->qtd_subtarefas;
                temp.subtarefas = atual->subtarefas;

                // Atual recebe próximo
                atual->id = atual->proxima->id;
                memcpy(atual->descricao, atual->proxima->descricao, sizeof(atual->descricao));
                atual->prazo = atual->proxima->prazo;
                atual->prioridade = atual->proxima->prioridade;
                atual->porcentagem_conclusao = atual->proxima->porcentagem_conclusao;
                atual->qtd_subtarefas = atual->proxima->qtd_subtarefas;
                atual->subtarefas = atual->proxima->subtarefas;

                // Próximo recebe temp
                atual->proxima->id = temp.id;
                memcpy(atual->proxima->descricao, temp.descricao, sizeof(atual->proxima->descricao));
                atual->proxima->prazo = temp.prazo;
                atual->proxima->prioridade = temp.prioridade;
                atual->proxima->porcentagem_conclusao = temp.porcentagem_conclusao;
                atual->proxima->qtd_subtarefas = temp.qtd_subtarefas;
                atual->proxima->subtarefas = temp.subtarefas;

                trocou = 1;
            }
            atual = atual->proxima;
        }
        ultimo_verificado = atual;
    } while (trocou);
}

void ordenarcao_por_prazo(Tarefa** lista_geral) {
    if (lista_geral == NULL || *lista_geral == NULL || (*lista_geral)->proxima == NULL) {
        return;
    }

    int trocou;
    Tarefa* atual;
    Tarefa* ultimo_verificado = NULL;

    do {
        trocou = 0;
        atual = *lista_geral;

        while (atual->proxima != ultimo_verificado) {
            // Ordena crescente por prazo: mais urgente (menor data) primeiro
            if (comparar_datas(atual->prazo, atual->proxima->prazo) > 0) {
                Tarefa temp;

                temp.id = atual->id;
                memcpy(temp.descricao, atual->descricao, sizeof(temp.descricao));
                temp.prazo = atual->prazo;
                temp.prioridade = atual->prioridade;
                temp.porcentagem_conclusao = atual->porcentagem_conclusao;
                temp.qtd_subtarefas = atual->qtd_subtarefas;
                temp.subtarefas = atual->subtarefas;

                atual->id = atual->proxima->id;
                memcpy(atual->descricao, atual->proxima->descricao, sizeof(atual->descricao));
                atual->prazo = atual->proxima->prazo;
                atual->prioridade = atual->proxima->prioridade;
                atual->porcentagem_conclusao = atual->proxima->porcentagem_conclusao;
                atual->qtd_subtarefas = atual->proxima->qtd_subtarefas;
                atual->subtarefas = atual->proxima->subtarefas;

                atual->proxima->id = temp.id;
                memcpy(atual->proxima->descricao, temp.descricao, sizeof(atual->proxima->descricao));
                atual->proxima->prazo = temp.prazo;
                atual->proxima->prioridade = temp.prioridade;
                atual->proxima->porcentagem_conclusao = temp.porcentagem_conclusao;
                atual->proxima->qtd_subtarefas = temp.qtd_subtarefas;
                atual->proxima->subtarefas = temp.subtarefas;

                trocou = 1;
            }
            atual = atual->proxima;
        }
        ultimo_verificado = atual;
    } while (trocou);
}

/* ============================================================================
 * MECANISMO RECURSIVO DE CÁLCULO E ATUALIZAÇÃO
 *
 * Justificativa Acadêmica (conforme Seção 5 do Escopo):
 * A recursividade percorre a lista encadeada de subtarefas aproveitando a pilha
 * de execução do compilador. Cada frame acumula o peso relativo da subtarefa
 * (100.0f / total) quando concluída. Como cada tarefa possui no máximo
 * MAX_SUBTAREFAS (10 nós), a profundidade de recursão é O(N) com N <= 10,
 * garantindo ausência total de stack overflow e extrema clareza algorítmica.
 * ============================================================================ */

float calculo_progresso(int id_tarefa_pai, const Tarefa* tarefa, const Subtarefa* sub) {
    (void)id_tarefa_pai; // Compatibilidade com a assinatura do escopo

    // Caso Base 1: Se a tarefa não possui subtarefas ou o ponteiro da lista é nulo
    if (tarefa == NULL || tarefa->qtd_subtarefas == 0 || sub == NULL) {
        return 0.0f;
    }

    // Parcela de contribuição deste nó para o progresso geral
    float peso_por_subtarefa = 100.0f / (float)tarefa->qtd_subtarefas;
    float parcela_deste_no = (sub->concluida == 1) ? peso_por_subtarefa : 0.0f;

    // Passo Recursivo: soma a parcela atual com a resolução recursiva do restante da lista ligada
    return parcela_deste_no + calculo_progresso(id_tarefa_pai, tarefa, sub->proxima);
}

void atualizaConclusao(Tarefa* tarefa, const Subtarefa* sub) {
    if (tarefa == NULL) return;

    if (tarefa->qtd_subtarefas == 0 || tarefa->subtarefas == NULL) {
        tarefa->porcentagem_conclusao = 0.0f;
        return;
    }

    const Subtarefa* inicio = (sub != NULL) ? sub : tarefa->subtarefas;
    tarefa->porcentagem_conclusao = calculo_progresso(tarefa->id, tarefa, inicio);

    // Arredondamento de precisão (evita 99.9999% por arredondamento de float)
    if (tarefa->porcentagem_conclusao > 99.99f) {
        tarefa->porcentagem_conclusao = 100.0f;
    }
}

//GESTÃO DE MEMÓRIA DINÂMICA 

void liberar_subtarefas(Subtarefa* lista) {
    Subtarefa* atual = lista;
    while (atual != NULL) {
        Subtarefa* proxima = atual->proxima;
        free(atual);
        atual = proxima;
    }
}

void liberar_todas_tarefas(Tarefa** lista) {
    if (lista == NULL || *lista == NULL) return;

    Tarefa* atual = *lista;
    while (atual != NULL) {
        Tarefa* proxima = atual->proxima;
        liberar_subtarefas(atual->subtarefas);
        free(atual);
        atual = proxima;
    }
    *lista = NULL;
}
