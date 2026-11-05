/**
 * ============================================================================
 * PROJETO PRÁTICO INTEGRADOR — TEMA T5: Gerenciador de Tarefas com Subtarefas
 * Diretório: recursividade-em-c/src/arquivos.c
 * Módulo 3: Implementação da Persistência Binária e Exportação de Relatórios
 * ============================================================================
 */

#ifdef __has_include
  #if __has_include("../include/arquivos.h")
    #include "../include/arquivos.h"
  #else
    #include "arquivos.h"
  #endif
#else
  #include "../include/arquivos.h"
#endif

int verificar_existencia_arquivo(const char* nome_arquivo) {
    if (nome_arquivo == NULL) return 0;
    FILE* fp = fopen(nome_arquivo, "rb");
    if (fp != NULL) {
        fclose(fp);
        return 1;
    }
    return 0;
}

void formatar_nome_arquivo_agenda(const char* data_entrada, char* buffer, int tamanho_buffer) {
    if (data_entrada == NULL || buffer == NULL || tamanho_buffer == 0) return;

    Data d = parse_data(data_entrada);
    snprintf(buffer, tamanho_buffer, "agenda_%04d-%02d-%02d.txt", d.ano, d.mes, d.dia);
}

int guardar_tarefas_binario(const Tarefa* lista, const char* nome_arquivo) {
    if (nome_arquivo == NULL) {
        return 0;
    }

    FILE* fp = fopen(nome_arquivo, "wb");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo para escrita binaria");
        return 0;
    }

    // 1. Escreve cabeçalho de integridade e versão
    char magic[5] = MAGIC_HEADER;
    int versao = VERSAO_ARQUIVO;
    if (fwrite(magic, sizeof(char), 4, fp) != 4 ||
        fwrite(&versao, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    // 2. Conta o número total de tarefas na lista ligada
    int total_tarefas = 0;
    const Tarefa* t = lista;
    while (t != NULL) {
        total_tarefas++;
        t = t->proxima;
    }

    if (fwrite(&total_tarefas, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return 0;
    }

    // 3. Itera pelas tarefas e grava dados estáticos + subtarefas vinculadas
    t = lista;
    while (t != NULL) {
        RegistroTarefaDisco reg_t;
        reg_t.id = t->id;
        strncpy(reg_t.descricao, t->descricao, 100);
        reg_t.descricao[100] = '\0';
        reg_t.prazo = t->prazo;
        reg_t.prioridade = t->prioridade;
        reg_t.percentual_conclusao = t->percentual_conclusao;
        reg_t.qtd_subtarefas = t->qtd_subtarefas;

        if (fwrite(&reg_t, sizeof(RegistroTarefaDisco), 1, fp) != 1) {
            fclose(fp);
            return 0;
        }

        // Grava as subtarefas desta tarefa
        const Subtarefa* s = t->subtarefas;
        while (s != NULL) {
            RegistroSubtarefaDisco reg_s;
            reg_s.id = s->id;
            strncpy(reg_s.descricao, s->descricao, 100);
            reg_s.descricao[100] = '\0';
            reg_s.concluida = s->concluida;
            reg_s.id_tarefa_pai = s->id_tarefa_pai;

            if (fwrite(&reg_s, sizeof(RegistroSubtarefaDisco), 1, fp) != 1) {
                fclose(fp);
                return 0;
            }

            s = s->proxima;
        }

        t = t->proxima;
    }

    fclose(fp);
    return 1;
}

Tarefa* carregar_tarefas_binario(const char* nome_arquivo) {
    if (nome_arquivo == NULL) {
        return NULL;
    }

    FILE* fp = fopen(nome_arquivo, "rb");
    if (fp == NULL) {
        return NULL;
    }

    // 1. Valida cabeçalho
    char magic[5] = {0};
    int versao = 0;
    if (fread(magic, sizeof(char), 4, fp) != 4 ||
        fread(&versao, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }

    if (strncmp(magic, MAGIC_HEADER, 4) != 0 || versao != VERSAO_ARQUIVO) {
        fprintf(stderr, "Arquivo binario com formato invalido ou versao incompativel!\n");
        fclose(fp);
        return NULL;
    }

    // 2. Lê quantidade total de tarefas
    int total_tarefas = 0;
    if (fread(&total_tarefas, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return NULL;
    }

    Tarefa* cabeca = NULL;
    Tarefa* cauda = NULL;

    // 3. Reconstrói a lista ligada de tarefas e suas respectivas subtarefas
    for (int i = 0; i < total_tarefas; i++) {
        RegistroTarefaDisco reg_t;
        if (fread(&reg_t, sizeof(RegistroTarefaDisco), 1, fp) != 1) {
            break;
        }

        Tarefa* nova_tarefa = (Tarefa*)malloc(sizeof(Tarefa));
        if (nova_tarefa == NULL) {
            break;
        }

        nova_tarefa->id = reg_t.id;
        strncpy(nova_tarefa->descricao, reg_t.descricao, 101);
        nova_tarefa->prazo = reg_t.prazo;
        nova_tarefa->prioridade = reg_t.prioridade;
        nova_tarefa->percentual_conclusao = reg_t.percentual_conclusao;
        nova_tarefa->qtd_subtarefas = reg_t.qtd_subtarefas;
        nova_tarefa->subtarefas = NULL;
        nova_tarefa->proxima = NULL;

        Subtarefa* sub_cauda = NULL;
        for (int j = 0; j < reg_t.qtd_subtarefas; j++) {
            RegistroSubtarefaDisco reg_s;
            if (fread(&reg_s, sizeof(RegistroSubtarefaDisco), 1, fp) != 1) {
                break;
            }

            Subtarefa* nova_sub = (Subtarefa*)malloc(sizeof(Subtarefa));
            if (nova_sub == NULL) {
                break;
            }

            nova_sub->id = reg_s.id;
            strncpy(nova_sub->descricao, reg_s.descricao, 101);
            nova_sub->concluida = reg_s.concluida;
            nova_sub->id_tarefa_pai = reg_s.id_tarefa_pai;
            nova_sub->proxima = NULL;

            if (nova_tarefa->subtarefas == NULL) {
                nova_tarefa->subtarefas = nova_sub;
            } else {
                sub_cauda->proxima = nova_sub;
            }
            sub_cauda = nova_sub;
        }

        atualizaConclusao(nova_tarefa, nova_tarefa->subtarefas);

        if (cabeca == NULL) {
            cabeca = nova_tarefa;
        } else {
            cauda->proxima = nova_tarefa;
        }
        cauda = nova_tarefa;
    }

    fclose(fp);
    return cabeca;
}

int exportar_agenda_dia_txt(const Tarefa* lista, const char* data, const char* nome_arquivo_saida) {
    if (data == NULL || nome_arquivo_saida == NULL) {
        return 0;
    }

    Data data_filtro = parse_data(data);
    char data_str[16];
    formatar_data(data_filtro, data_str, sizeof(data_str));

    FILE* fp = fopen(nome_arquivo_saida, "w");
    if (fp == NULL) {
        perror("Erro ao criar arquivo de agenda");
        return 0;
    }

    fprintf(fp, "================================================================================\n");
    fprintf(fp, "                     AGENDA DO DIA: %s\n", data_str);
    fprintf(fp, "         Sistema Gerenciador de Tarefas com Subtarefas (Tema T5)\n");
    fprintf(fp, "================================================================================\n\n");

    int encontradas = 0;
    const Tarefa* t = lista;

    while (t != NULL) {
        if (t->prazo.dia == data_filtro.dia &&
            t->prazo.mes == data_filtro.mes &&
            t->prazo.ano == data_filtro.ano) {
            
            encontradas++;
            const char* prio_txt = (t->prioridade == PRIORIDADE_ALTA) ? "ALTA" :
                                   (t->prioridade == PRIORIDADE_MEDIA) ? "MEDIA" : "BAIXA";

            fprintf(fp, "--------------------------------------------------------------------------------\n");
            fprintf(fp, "[TAREFA #%02d] %s\n", t->id, t->descricao);
            fprintf(fp, "  Prioridade: %-6s | Progresso: %.1f%% | Subtarefas: %d/%d\n",
                    prio_txt, t->percentual_conclusao, t->qtd_subtarefas, MAX_SUBTAREFAS);
            
            if (t->subtarefas != NULL) {
                fprintf(fp, "  Subtarefas vinculadas:\n");
                const Subtarefa* s = t->subtarefas;
                while (s != NULL) {
                    fprintf(fp, "    [%c] (%d.%d) %s\n",
                            s->concluida ? 'X' : ' ',
                            s->id_tarefa_pai, s->id, s->descricao);
                    s = s->proxima;
                }
            } else {
                fprintf(fp, "  (Nenhuma subtarefa associada)\n");
            }
            fprintf(fp, "\n");
        }
        t = t->proxima;
    }

    if (encontradas == 0) {
        fprintf(fp, "Nenhuma tarefa agendada para o dia %s.\n", data_str);
    } else {
        fprintf(fp, "Total de tarefas encontradas para esta data: %d\n", encontradas);
    }

    fprintf(fp, "\n================================================================================\n");
    fprintf(fp, "Arquivo gerado automaticamente pelo modulo arquivos.c\n");
    fprintf(fp, "================================================================================\n");

    fclose(fp);
    return encontradas;
}

int exportar_relatorio_geral_txt(const Tarefa* lista, const char* nome_arquivo_saida) {
    if (nome_arquivo_saida == NULL) {
        return 0;
    }

    FILE* fp = fopen(nome_arquivo_saida, "w");
    if (fp == NULL) {
        perror("Erro ao criar arquivo de relatorio");
        return 0;
    }

    fprintf(fp, "================================================================================\n");
    fprintf(fp, "                  RELATORIO GERAL DE TAREFAS E SUBTAREFAS\n");
    fprintf(fp, "                 Algoritmos e Programacao de Computadores em C\n");
    fprintf(fp, "================================================================================\n\n");

    int total_tarefas = 0;
    int total_subtarefas = 0;
    int total_concluidas = 0;

    const Tarefa* t = lista;
    char data_str[16];

    while (t != NULL) {
        total_tarefas++;
        formatar_data(t->prazo, data_str, sizeof(data_str));
        const char* prio_txt = (t->prioridade == PRIORIDADE_ALTA) ? "ALTA" :
                               (t->prioridade == PRIORIDADE_MEDIA) ? "MEDIA" : "BAIXA";

        fprintf(fp, "================================================================================\n");
        fprintf(fp, "TAREFA [%02d]: %s\n", t->id, t->descricao);
        fprintf(fp, "Prazo: %s | Prioridade: %s | Progresso: %.1f%%\n",
                data_str, prio_txt, t->percentual_conclusao);
        fprintf(fp, "Subtarefas cadastradas: %d / %d\n", t->qtd_subtarefas, MAX_SUBTAREFAS);
        fprintf(fp, "--------------------------------------------------------------------------------\n");

        if (t->subtarefas == NULL) {
            fprintf(fp, "  (Sem subtarefas)\n");
        } else {
            const Subtarefa* s = t->subtarefas;
            while (s != NULL) {
                total_subtarefas++;
                if (s->concluida) total_concluidas++;

                fprintf(fp, "  [%c] Subtarefa #%d.%d: %s\n",
                        s->concluida ? 'X' : ' ',
                        s->id_tarefa_pai, s->id, s->descricao);
                s = s->proxima;
            }
        }
        fprintf(fp, "\n");
        t = t->proxima;
    }

    fprintf(fp, "================================================================================\n");
    fprintf(fp, "RESUMO ESTATISTICO DO SISTEMA:\n");
    fprintf(fp, "  Total de Tarefas Principais: %d\n", total_tarefas);
    fprintf(fp, "  Total de Subtarefas:         %d\n", total_subtarefas);
    fprintf(fp, "  Subtarefas Concluidas:       %d\n", total_concluidas);
    fprintf(fp, "  Taxa Global de Conclusao:    %.1f%%\n",
            (total_subtarefas > 0) ? ((float)total_concluidas / total_subtarefas) * 100.0f : 0.0f);
    fprintf(fp, "================================================================================\n");

    fclose(fp);
    return 1;
}
