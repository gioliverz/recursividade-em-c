/**
 * ============================================================================
 * PROJETO PRÁTICO INTEGRADOR — TEMA T5: Gerenciador de Tarefas com Subtarefas
 * Diretório: recursividade-em-c/src/menu.c
 * Módulo 1: Implementação de Menus Interativos, Validações e Navegação
 * ============================================================================
 */

#ifdef __has_include
  #if __has_include("../include/menu.h")
    #include "../include/menu.h"
  #else
    #include "menu.h"
  #endif
#else
  #include "../include/menu.h"
#endif

static int proximo_id_tarefa = 1;

void definir_proximo_id(int id) {
    if (id > proximo_id_tarefa) {
        proximo_id_tarefa = id;
    }
}

void limpar_tela(void) {
#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#else
    system("clear");
#endif
}

void pausar_tela(void) {
    printf("\nPressione [ENTER] para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void ler_string_segura(char* buffer, int tamanho) {
    if (buffer == NULL || tamanho <= 0) return;
    if (fgets(buffer, tamanho, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    } else {
        buffer[0] = '\0';
    }
}

int ler_inteiro(const char* rotulo) {
    char linha[64];
    int valor = 0;
    while (1) {
        printf("%s", rotulo);
        if (fgets(linha, sizeof(linha), stdin) != NULL) {
            if (sscanf(linha, "%d", &valor) == 1) {
                return valor;
            }
        }
        printf("Valor invalido! Por favor digite um numero inteiro valido.\n");
    }
}

void exibir_mensagem(const char* mensagem) {
    printf("\n>>> %s <<<\n", mensagem);
}

void exibir_menu_principal(void) {
    printf("=========================================================\n");
    printf("   GERENCIADOR DE TAREFAS EM LINGUAGEM C    \n");
    printf("=========================================================\n");
    printf("  [1] Gerenciar Tarefas Principais\n");
    printf("  [2] Gerenciar Subtarefas\n");
    printf("  [3] Filtros e Ordenacoes (Prioridade / Prazo)\n");
    printf("  [4] Salvar Dados em Arquivo Binario (dados.bin)\n");
    printf("  [5] Carregar Dados do Arquivo Binario\n");
    printf("  [6] Exportar Agenda do Dia (.txt)\n");
    printf("  [7] Exportar Relatorio Geral (.txt)\n");
    printf("  [0] Sair do Sistema\n");
    printf("=========================================================\n");
}

void exibir_menu_tarefas(void) {
    printf("\n--- GERENCIAMENTO DE TAREFAS ---\n");
    printf("  [1] Cadastrar Nova Tarefa\n");
    printf("  [2] Listar Todas as Tarefas\n");
    printf("  [3] Ver Detalhes de uma Tarefa\n");
    printf("  [4] Editar Tarefa\n");
    printf("  [5] Remover Tarefa\n");
    printf("  [0] Voltar ao Menu Principal\n");
}

void exibir_menu_subtarefas(void) {
    printf("\n--- GERENCIAMENTO DE SUBTAREFAS ---\n");
    printf("  [1] Adicionar Subtarefa a uma Tarefa\n");
    printf("  [2] Atualizar Progresso / Conclusao de Subtarefa\n");
    printf("  [3] Editar Descricao de Subtarefa\n");
    printf("  [4] Remover Subtarefa\n");
    printf("  [5] Listar Subtarefas de uma Tarefa\n");
    printf("  [0] Voltar ao Menu Principal\n");
}

void exibir_filtro(void) {
    printf("\n--- FILTROS E ORDENACOES ---\n");
    printf("  [1] Ordenar Tarefas por Prioridade (Alta -> Baixa)\n");
    printf("  [2] Ordenar Tarefas por Prazo (Mais Urgente Primeiro)\n");
    printf("  [3] Buscar Tarefa por ID\n");
    printf("  [0] Voltar ao Menu Principal\n");
}

void menu_cadastrar_tarefa(Tarefa** lista_geral) {
    if (lista_geral == NULL) return;

    printf("\n>>> CADASTRO DE NOVA TAREFA <<<\n");
    char desc[101];
    char prazo[32];
    int prioridade;

    printf("Descricao da Tarefa (ate 100 caracteres): ");
    ler_string_segura(desc, sizeof(desc));

    printf("Prazo de conclusao (DD/MM/AAAA): ");
    ler_string_segura(prazo, sizeof(prazo));

    printf("Prioridade (1 - Baixa, 2 - Media, 3 - Alta): ");
    prioridade = ler_inteiro("");

    Tarefa* nova = criar_tarefa(proximo_id_tarefa++, desc, prazo, prioridade);
    if (nova == NULL) {
        exibir_mensagem("Erro ao alocar memoria para a nova tarefa!");
        return;
    }

    if (*lista_geral == NULL) {
        *lista_geral = nova;
    } else {
        Tarefa* aux = *lista_geral;
        while (aux->proxima != NULL) {
            aux = aux->proxima;
        }
        aux->proxima = nova;
    }

    exibir_mensagem("Tarefa cadastrada com sucesso!");
    printf("ID gerado: %d\n", nova->id);
}

void menu_adicionar_subtarefa(Tarefa* lista) {
    if (lista == NULL) {
        exibir_mensagem("Nao ha tarefas cadastradas para adicionar subtarefas!");
        return;
    }

    printf("\n>>> ADICIONAR SUBTAREFA <<<\n");
    int id_pai = ler_inteiro("Informe o ID da Tarefa Mae: ");
    Tarefa* pai = busca_ID(lista, id_pai);

    if (pai == NULL) {
        exibir_mensagem("Tarefa mae nao encontrada com esse ID!");
        return;
    }

    if (pai->qtd_subtarefas >= MAX_SUBTAREFAS) {
        printf("\n[AVISO] Limite maximo operacional de %d subtarefas atingido para a tarefa #%d!\n",
               MAX_SUBTAREFAS, pai->id);
        return;
    }

    char desc_sub[101];
    printf("Descricao da Subtarefa: ");
    ler_string_segura(desc_sub, sizeof(desc_sub));

    int novo_sub_id = pai->qtd_subtarefas + 1;
    int res = criar_sub(lista, id_pai, novo_sub_id, desc_sub, 0);

    if (res == 1) {
        exibir_mensagem("Subtarefa vinculada com sucesso! Status inicial: Pendente (0%)");
        printf("Progresso recalculado da Tarefa #%d: %.1f%%\n", pai->id, pai->percentual_conclusao);
    } else {
        exibir_mensagem("Erro ao vincular subtarefa!");
    }
}

void menu_atualizar_progresso_subtarefa(Tarefa* lista) {
    if (lista == NULL) {
        exibir_mensagem("Nenhuma tarefa disponivel no sistema!");
        return;
    }

    printf("\n>>> ATUALIZAR STATUS DE SUBTAREFA <<<\n");
    int id_pai = ler_inteiro("ID da Tarefa Mae: ");
    Tarefa* pai = busca_ID(lista, id_pai);

    if (pai == NULL) {
        exibir_mensagem("Tarefa nao encontrada!");
        return;
    }

    if (pai->subtarefas == NULL) {
        exibir_mensagem("Esta tarefa nao possui subtarefas!");
        return;
    }

    listar_sub(pai);
    int id_sub = ler_inteiro("Numero da Subtarefa a alterar: ");

    printf("Novo status (0 para Pendente, 1 para Concluida): ");
    int novo_status = ler_inteiro("");

    int res = editar_sub(lista, id_pai, id_sub, NULL, (float)novo_status);
    if (res) {
        exibir_mensagem("Status da subtarefa atualizado!");
        printf("Novo percentual recursivo da tarefa #%d: %.1f%%\n", pai->id, pai->percentual_conclusao);
    } else {
        exibir_mensagem("Subtarefa nao encontrada!");
    }
}

void menu_exportar_agenda(Tarefa* lista) {
    if (lista == NULL) {
        exibir_mensagem("Nenhuma tarefa para exportar!");
        return;
    }

    printf("\n>>> EXPORTAR AGENDA DO DIA <<<\n");
    char data_str[32];
    char nome_arquivo[64];

    printf("Informe a data para a agenda (DD/MM/AAAA): ");
    ler_string_segura(data_str, sizeof(data_str));

    formatar_nome_arquivo_agenda(data_str, nome_arquivo, sizeof(nome_arquivo));
    int total = exportar_agenda_dia_txt(lista, data_str, nome_arquivo);

    if (total >= 0) {
        printf("\nAgenda gerada com sucesso no arquivo: '%s'\n", nome_arquivo);
        printf("Total de tarefas agendadas encontradas: %d\n", total);
    } else {
        exibir_mensagem("Falha ao gerar arquivo de agenda!");
    }
}

void mostrar_lista_tarefas(const Tarefa* lista) {
    listar_tarefas(lista);
}

void mostrar_detalhes_tarefa(Tarefa** lista_global, int id) {
    if (lista_global == NULL || *lista_global == NULL) {
        exibir_mensagem("Nenhuma tarefa cadastrada!");
        return;
    }

    Tarefa* t = busca_ID(*lista_global, id);
    if (t == NULL) {
        exibir_mensagem("Tarefa nao encontrada!");
        return;
    }

    char data_str[16];
    formatar_data(t->prazo, data_str, sizeof(data_str));
    const char* prio_txt = (t->prioridade == PRIORIDADE_ALTA) ? "Alta (3)" :
                           (t->prioridade == PRIORIDADE_MEDIA) ? "Media (2)" : "Baixa (1)";

    printf("\n=======================================================\n");
    printf("DETALHES DA TAREFA #%d\n", t->id);
    printf("=======================================================\n");
    printf("Descricao:   %s\n", t->descricao);
    printf("Prazo:       %s\n", data_str);
    printf("Prioridade:  %s\n", prio_txt);
    printf("Progresso:   %.1f%% (calculado recursivamente)\n", t->percentual_conclusao);
    printf("Subtarefas:  %d de %d permitidas\n", t->qtd_subtarefas, MAX_SUBTAREFAS);
    printf("-------------------------------------------------------\n");

    listar_sub(t);
    printf("=======================================================\n");
}

void executar_opcao_principal(Tarefa** lista_geral, int opcao) {
    switch (opcao) {
        case 1: {
            int op_t;
            do {
                exibir_menu_tarefas();
                op_t = ler_inteiro("Opcao: ");
                switch (op_t) {
                    case 1:
                        menu_cadastrar_tarefa(lista_geral);
                        break;
                    case 2:
                        mostrar_lista_tarefas(*lista_geral);
                        break;
                    case 3: {
                        int id = ler_inteiro("Informe o ID da Tarefa: ");
                        mostrar_detalhes_tarefa(lista_geral, id);
                        break;
                    }
                    case 4: {
                        int id = ler_inteiro("Informe o ID da Tarefa a editar: ");
                        char n_desc[101], n_prazo[32];
                        printf("Nova Descricao (deixe vazio para manter): ");
                        ler_string_segura(n_desc, sizeof(n_desc));
                        printf("Novo Prazo DD/MM/AAAA (deixe vazio para manter): ");
                        ler_string_segura(n_prazo, sizeof(n_prazo));
                        int n_prio = ler_inteiro("Nova Prioridade 1-3 (0 para manter): ");
                        if (editar_tarefa(*lista_geral, id, n_desc, n_prazo, n_prio)) {
                            exibir_mensagem("Tarefa atualizada com sucesso!");
                        } else {
                            exibir_mensagem("Tarefa nao encontrada!");
                        }
                        break;
                    }
                    case 5: {
                        int id = ler_inteiro("Informe o ID da Tarefa a remover: ");
                        if (remover_tarefa(lista_geral, id)) {
                            exibir_mensagem("Tarefa e subtarefas removidas com sucesso da memoria!");
                        } else {
                            exibir_mensagem("Tarefa nao encontrada!");
                        }
                        break;
                    }
                }
            } while (op_t != 0);
            break;
        }

        case 2: {
            int op_s;
            do {
                exibir_menu_subtarefas();
                op_s = ler_inteiro("Opcao: ");
                switch (op_s) {
                    case 1:
                        menu_adicionar_subtarefa(*lista_geral);
                        break;
                    case 2:
                        menu_atualizar_progresso_subtarefa(*lista_geral);
                        break;
                    case 3: {
                        int id_p = ler_inteiro("ID da Tarefa Mae: ");
                        int id_s = ler_inteiro("ID da Subtarefa: ");
                        char n_desc[101];
                        printf("Nova Descricao: ");
                        ler_string_segura(n_desc, sizeof(n_desc));
                        if (editar_sub(*lista_geral, id_p, id_s, n_desc, -1.0f)) {
                            exibir_mensagem("Descricao da subtarefa atualizada!");
                        } else {
                            exibir_mensagem("Subtarefa nao encontrada!");
                        }
                        break;
                    }
                    case 4: {
                        int id_p = ler_inteiro("ID da Tarefa Mae: ");
                        int id_s = ler_inteiro("ID da Subtarefa a remover: ");
                        if (remover_sub(*lista_geral, id_p, id_s)) {
                            exibir_mensagem("Subtarefa removida e percentual recalculado!");
                        } else {
                            exibir_mensagem("Subtarefa nao encontrada!");
                        }
                        break;
                    }
                    case 5: {
                        int id_p = ler_inteiro("ID da Tarefa Mae: ");
                        Tarefa* p = busca_ID(*lista_geral, id_p);
                        if (p != NULL) {
                            listar_sub(p);
                        } else {
                            exibir_mensagem("Tarefa nao encontrada!");
                        }
                        break;
                    }
                }
            } while (op_s != 0);
            break;
        }

        case 3: {
            int op_f;
            do {
                exibir_filtro();
                op_f = ler_inteiro("Opcao: ");
                switch (op_f) {
                    case 1:
                        ordenacao_por_prioridade(lista_geral);
                        exibir_mensagem("Lista ordenada por Prioridade (Alta -> Media -> Baixa)!");
                        mostrar_lista_tarefas(*lista_geral);
                        break;
                    case 2:
                        ordenarcao_por_prazo(lista_geral);
                        exibir_mensagem("Lista ordenada por Prazo cronologico (Mais urgente primeiro)!");
                        mostrar_lista_tarefas(*lista_geral);
                        break;
                    case 3: {
                        int id = ler_inteiro("Digite o ID para busca: ");
                        mostrar_detalhes_tarefa(lista_geral, id);
                        break;
                    }
                }
            } while (op_f != 0);
            break;
        }

        case 4: {
            if (guardar_tarefas_binario(*lista_geral, "dados.bin")) {
                exibir_mensagem("Dados salvos com sucesso no arquivo binario 'dados.bin'!");
            } else {
                exibir_mensagem("Falha ao salvar dados no arquivo binario!");
            }
            break;
        }

        case 5: {
            if (verificar_existencia_arquivo("dados.bin")) {
                Tarefa* carregada = carregar_tarefas_binario("dados.bin");
                if (carregada != NULL) {
                    liberar_todas_tarefas(lista_geral);
                    *lista_geral = carregada;
                    Tarefa* t = *lista_geral;
                    int max_id = 0;
                    while (t != NULL) {
                        if (t->id > max_id) max_id = t->id;
                        t = t->proxima;
                    }
                    definir_proximo_id(max_id + 1);
                    exibir_mensagem("Dados carregados com sucesso do arquivo binario 'dados.bin'!");
                    mostrar_lista_tarefas(*lista_geral);
                } else {
                    exibir_mensagem("Erro ao processar dados do arquivo binario!");
                }
            } else {
                exibir_mensagem("O arquivo binario 'dados.bin' ainda nao existe no disco!");
            }
            break;
        }

        case 6: {
            menu_exportar_agenda(*lista_geral);
            break;
        }

        case 7: {
            if (exportar_relatorio_geral_txt(*lista_geral, "relatorio_geral.txt")) {
                exibir_mensagem("Relatorio geral salvo em 'relatorio_geral.txt'!");
            } else {
                exibir_mensagem("Erro ao gerar relatorio geral!");
            }
            break;
        }

        case 0:
            printf("\nEncerrando o sistema e liberando memoria...\n");
            break;

        default:
            exibir_mensagem("Opcao invalida!");
            break;
    }
}
