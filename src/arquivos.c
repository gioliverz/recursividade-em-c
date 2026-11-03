#include <stdio.h>
#include <stdlib.h>
#include "../include/arquivos.h"

int salvar_tarefas_binario(const char* nome_arquivo, const Tarefa* lista) {
    FILE* arq = fopen(nome_arquivo, "wb");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo binario para escrita");
        return 0; // Falha na abertura
    }

    const Tarefa* t = lista;
    while (t != NULL) {
        // 1. Grava a tarefa principal (escreve o bloco de dados da struct)
        fwrite(t, sizeof(Tarefa), 1, arq);

        // 2. Grava cada subtarefa associada sequencialmente
        const Subtarefa* sub = t->subtarefas;
        while (sub != NULL) {
            fwrite(sub, sizeof(Subtarefa), 1, arq);
            sub = sub->proxima;
        }

        t = t->proxima;
    }

    fclose(arq);
    return 1; // Sucesso
}

int carregar_tarefas_binario(const char* nome_arquivo, Tarefa** lista_geral) {
    FILE* arq = fopen(nome_arquivo, "rb");
    if (arq == NULL) {
        // Se o arquivo não existir ainda (primeira execução), apenas retorna sucesso/alerta
        return 0; 
    }

    // Garante que a lista atual está limpa antes de carregar
    liberar_todas_tarefas(lista_geral);

    Tarefa temp_t;
    // Lê cada tarefa principal do arquivo
    while (fread(&temp_t, sizeof(Tarefa), 1, arq) == 1) {
        // Recria a tarefa principal na memória
        Tarefa* nova_t = criar_tarefa(temp_t.id, temp_t.descricao, "01/01/2026", temp_t.prioridade);
        if (nova_t != NULL) {
            nova_t->prazo = temp_t.prazo; // Copia a data correta
            nova_t->porcentagem_conclusao = temp_t.porcentagem_conclusao;

            // Lê a quantidade exata de subtarefas gravadas para esta tarefa
            for (int i = 0; i < temp_t.qtd_subtarefas; i++) {
                Subtarefa temp_sub;
                if (fread(&temp_sub, sizeof(Subtarefa), 1, arq) == 1) {
                    criar_sub(nova_t, nova_t->id, temp_sub.id, temp_sub.descricao, temp_sub.concluida);
                }
            }

            // Insere na lista geral (ou chama função auxiliar de inserção)
            nova_t->proxima = *lista_geral;
            *lista_geral = nova_t;
        }
    }

    fclose(arq);
    return 1; // Sucesso
}

int exportar_agenda_dia_txt(const char* nome_arquivo, const Tarefa* lista, Data data_alvo) {
    FILE* arq = fopen(nome_arquivo, "w");
    if (arq == NULL) {
        perror("Erro ao criar arquivo de exportacao da agenda");
        return 0;
    }

    fprintf(arq, "===============================================\n");
    fprintf(arq, " AGENDA DO DIA: %02d/%02d/%04d\n", data_alvo.dia, data_alvo.mes, data_alvo.ano);
    fprintf(arq, "===============================================\n\n");

    const Tarefa* t = lista;
    int encontradas = 0;

    while (t != NULL) {
        // Verifica se a data da tarefa coincide com a data alvo
        if (comparar_datas(t->prazo, data_alvo) == 0) {
            encontradas++;
            fprintf(arq, "[ID: %d] %s - Prioridade: %d - Conclusao: %.1f%%\n",
                    t->id, t->descricao, t->prioridade, t->porcentagem_conclusao);

            // Lista as subtarefas associadas no relatório
            const Subtarefa* sub = t->subtarefas;
            while (sub != NULL) {
                fprintf(arq, "   └─ [%s] Subtarefa #%d: %s\n",
                        sub->concluida ? "X" : " ", sub->id, sub->descricao);
                sub = sub->proxima;
            }
            fprintf(arq, "-----------------------------------------------\n");
        }
        t = t->proxima;
    }

    if (encontradas == 0) {
        fprintf(arq, "Nenhuma tarefa agendada para este dia.\n");
    }

    fclose(arq);
    return 1; // Sucesso
}