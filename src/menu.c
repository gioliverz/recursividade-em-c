#include <stdio.h>
#include <stdlib.h>
#include "menu.h"   // vira função do compilador (gcc) encontrar o arquivo
#include "tarefas.h" // para usar as funções de tarefas
#include "arquivos.h"

// --- Exibir menu principal ---
int exibir_menu_principal(void) {
    limpar_tela();
    int opcao;
    printf("==========================================\n");
    printf("          Gerenciador de Tarefas          \n");
    printf("==========================================\n");
    printf(" 1 -  Gerenciar Tarefas \n");
    printf(" 2 - Gerenciar Subtarefas \n");
    printf(" 3 - Filtrar / Ordenar Tarefas\n");
    printf(" 0 - Sair do Sistema\n");
    printf("------------------------------------------\n");
    printf("Digite a opcao desejada: ");

    if (scanf(" %d", &opcao) != 1) {
        while (getchar() != '\n'); // limpa buffer em caso de entrada inválida
        return -1;
    }
    return opcao;
}

// --- Executar opção do menu principal ---
void executar_opcao_principal(int opcao, Tarefa** lista_geral) {
    switch (opcao) {
        case 1:
            exibir_menu_tarefas(lista_geral);
            break;
        case 2:
            exibir_menu_subtarefas(lista_geral);
            break;
        case 3:
            exibir_filtro(lista_geral);
            break;
        case 0:
            printf("\nEncerrando o sistema...\n");
            break;
        default:
            printf("\nOpcao invalida! Por favor, tente novamente.\n");
            pausar_tela();
            break;
    }
}

// --- Exibir submenus ---

void exibir_menu_tarefas(Tarefa** lista_geral){
    int opcao;
    do {
        limpar_tela();
        printf("==========================================\n");
        printf("          Submenu: Gerenciar Tarefas       \n");
        printf("==========================================\n");
        printf(" 1 - Criar Tarefa\n");
        printf(" 2 - Listar Tarefas\n");
        printf(" 3 - Ver Detalhes de uma Tarefa\n");
        printf(" 4 - Editar Tarefa\n");
        printf(" 5 - Remover Tarefa\n");
        printf(" 0 - Voltar ao Menu Principal\n");
        printf("------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        scanf(" %d", &opcao);

        switch (opcao) {
            case 1:
                menu_cadastrar_tarefa(lista_geral);
                break;
            case 2:
                limpar_tela();
                listar_tarefas(*lista_geral);
                break;
            case 3:
                mostrar_detalhes_tarefa(lista_geral);
                break;
            case 4: {
                limpar_tela();
                int id;
                char desc[100], prazo[11];
                int prioridade;
                printf("Digite o ID da tarefa que deseja editar: ");
                scanf(" %d", &id);
                printf("Nova descricao: ");
                scanf(" %[^\n]", desc);
                printf("Novo prazo (dd/mm/aaaa): ");
                scanf(" %s", prazo);
                printf("Nova prioridade (1-Baixa, 2-Media, 3-Alta): ");
                scanf(" %d", &prioridade);

                if (editar_tarefa(*lista_geral, id, desc, prazo, prioridade)) {
                    exibir_mensagem("Tarefa editada com sucesso!");
                } else {
                    exibir_mensagem("Erro: Tarefa nao encontrada ou dados invalidos.");
                }
                pausar_tela();
                break;
            }
            case 5: {
                limpar_tela();
                int id;
                printf("Digite o ID da tarefa a ser removida: ");
                scanf(" %d", &id);
                if (remover_tarefa(lista_geral, id)) {
                    exibir_mensagem("Tarefa removida com sucesso!");
                } else {
                    exibir_mensagem("Erro: Tarefa nao encontrada.");
                }
                pausar_tela();
                break;
            }
        }
    } while (opcao != 0);
}

void exibir_menu_subtarefas(Tarefa** lista_geral){
    int opcao;
    do {
        limpar_tela();
        printf("==========================================\n");
        printf("          Submenu: Gerenciar Subtarefas    \n");
        printf("==========================================\n");
        printf(" 1 - Adicionar Subtarefa a uma Tarefa\n");
        printf(" 2 - Editar Subtarefa\n");
        printf(" 3 - Remover Subtarefa\n");
        printf(" 4 - Listar Subtarefas de uma Tarefa\n");
        printf(" 0 - Voltar ao Menu Principal\n");
        printf("------------------------------------------\n");
        printf("Digite a opcao desejada: ");
        scanf(" %d", &opcao);

        switch (opcao) {
            case 1:
                menu_adicionau_subtarefa(lista_geral);
                break;
            case 2:
                menu_editar_subtarefa(lista_geral);
                break;
            case 3: {       // chaves para criar escopo local (variáveis id_pai e id_sub)
                limpar_tela();
                int id_pai, id_sub;
                printf("Digite o ID da Tarefa: ");
                scanf(" %d", &id_pai);
                printf("Digite o ID da Subtarefa a remover: ");
                scanf(" %d", &id_sub);
                if (remover_sub(*lista_geral, id_pai, id_sub)) {
                    exibir_mensagem("Subtarefa removida com sucesso!");
                } else {
                    exibir_mensagem("Erro: Subtarefanão encontrada ou ID inválido.");
                }
                pausar_tela();
                break;
            }
        }
    } while (opcao != 0);
}

void exibir_filtro(Tarefa** lista_geral){
    limpar_tela();
    int opt_filtro;
    printf("==========================================\n");
    printf("          Submenu: Filtrar Tarefas         \n");
    printf("==========================================\n");
    printf(" 1 - Ordenar por Prioridade\n");
    printf(" 2 - Ordenar por Prazo\n");
    printf(" 0 - Voltar ao Menu Principal\n");
    printf("------------------------------------------\n");
    printf("Digite a opcao desejada: ");

    if (scanf(" %d", &opt_filtro) != 1) {
        while (getchar() != '\n'); // limpa o buffer se o usuário digitar letras
        return;
    }
    switch (opt_filtro) {
        case 1:
            ordenacao_por_prioridade(lista_geral);
            exibir_mensagem("Lista ordenada por prioridade com sucesso!");
            pausar_tela();
            break;
        case 2:
            ordenacao_por_prazo(lista_geral);
            exibir_mensagem("Lista ordenada por prazo com sucesso!");
            pausar_tela();
            break;
        case 0:
            break;
        default:
            exibir_mensagem("Opcao invalida! Retornando ao menu principal.");
            pausar_tela();
            break;
    }
}

void menu_cadastrar_tarefa(Tarefa** lista_geral) {
    limpar_tela();
    printf("==========================================\n");
    printf("           Cadastrar Nova Tarefa          \n");
    printf("==========================================\n");

    int id;
    char descricao[100];
    char prazo[11];
    int prioridade;

    // Geração de ID único
    if (*lista_geral == NULL) {
        id = 1; // Primeira tarefa
    } else {
        Tarefa* atual = *lista_geral;
        id = 1;
        while (atual != NULL) {
            if (atual->id >= id) {
                id = atual->id + 1;
            }
            atual = atual->proxima;
        }
    }
    printf("Digite a descricao (máximo de 100 caracteres): ");
    scanf(" %[^\n]", descricao);
    printf("Digite o prazo (dd/mm/aaaa): ");
    scanf(" %s", prazo);
    do {
        printf("Digite a prioridade (1-Baixa, 2-Media, 3-Alta): ");
        scanf(" %d", &prioridade);
        if (prioridade < 1 || prioridade > 3) {
            printf("Erro: Prioridade invalida! Digite apenas 1, 2 ou 3.\n");
        }
    } while (prioridade < 1 || prioridade > 3);


    Tarefa* nova_tarefa = criar_tarefa(id, descricao, prazo, prioridade);
    if (nova_tarefa != NULL) {
        nova_tarefa->proxima = *lista_geral;
        *lista_geral = nova_tarefa;
        printf("\nTarefa ID %d criada e inserida na lista geral!\n", id);
    } else {
        exibir_mensagem("Erro: Falha de memoria ao criar a tarefa.");
    }
    pausar_tela();
}

void menu_adicionar_subtarefa(Tarefa** lista_geral){
    limpar_tela();
    printf("==========================================\n");
    printf("             Adicionar Subtarefa          \n");
    printf("==========================================\n");

    int id_pai;
    char desc_sub[100];
    printf("Digite o ID da Tarefa a qual deseja adicionar a subtarefa: ");
    scanf(" %d", &id_pai);
    Tarefa* pai = busca_ID(*lista_geral, id_pai);
    if (pai == NULL) {
        exibir_mensagem("Erro: Tarefa nao encontrada.");
        pausar_tela();
        return;
    }
    if (pai->qtd_subtarefas >= MAX_SUBTAREFAS) {
        exibir_mensagem("Erro: Limite maximo de 10 subtarefas atingido!");
        pausar_tela();
        return;
    }
    printf("Digite a descricao da subtarefa (max 100 caracteres): ");
    scanf(" %[^\n]", desc_sub);
    int id_sub_gerado = pai->qtd_subtarefas + 1;
    if (criar_sub(*lista_geral, id_pai, id_sub_gerado, desc_sub, 0)) {
        exibir_mensagem("Subtarefa vinculada com sucesso!");
    } else {
        exibir_mensagem("Erro: Falha ao criar a subtarefa.");
    }
    pausar_tela();
}

void menu_atualizar_subtarefa(Tarefa** lista_geral){
    limpar_tela();
    printf("==========================================\n");
    printf("             Editar Subtarefa             \n");
    printf("==========================================\n");

    int id_pai, id_sub;
    char nova_desc[100];
    int nova_conclusao;
    printf("Digite o ID da Tarefa: ");
    scanf(" %d", &id_pai);
    printf("Digite o ID da Subtarefa a ser editada: ");
    scanf(" %d", &id_sub);
    printf("Digite a nova descricao da subtarefa (max 100 caracteres): ");
    scanf(" %[^\n]", nova_desc);
    printf("Subtarefa concluida? (0 para Nao, 1 para Sim): ");
    scanf(" %d", &nova_conclusao);

    if (editar_sub(*lista_geral, id_pai, id_sub, nova_desc, nova_conclusao)) {
        exibir_mensagem("Subtarefa editada com sucesso!");
    } else {
        exibir_mensagem("Erro: Subtarefa ou Tarefa não localizada.");
    }
    pausar_tela();
}

void mostrar_detalhes_tarefa(Tarefa** lista_geral){
    limpar_tela();
    int id;
    printf("Digite o ID da tarefa que deseja visualizar: ");
    scanf(" %d", &id);

    Tarefa* t = busca_ID(*lista_geral, id);
    if (t == NULL) {
        exibir_mensagem("Erro: Tarefa nao localizada.");
        pausar_tela();
        return;
    }
    limpar_tela();
    char buffer_data[50];
    formatar_data(t->prazo,buffer_data,sizeof(buffer_data));
    printf("==========================================\n");
    printf("            Detalhes da Tarefa            \n");
    printf("==========================================\n");
    printf("ID: %d\n", t->id);
    printf("Descricao: %s\n",t->descricao);
    printf("Prazo: %s\n",buffer_data);
    printf("Prioridade: %d\n",t->prioridade);
    printf("Progresso Atual: %.1f%%\n",t->percentual_conclusao);
    printf("Quantidade de Subtarefas: %d\n",t->qtd_subtarefas);
    printf("==========================================\n");
    pausar_tela();
}

void menu_exportar_agenda(Tarefa* lista){
    if (lista == NULL) {
        exibir_mensagem("Nao ha tarefas para exportar.");
        pausar_tela();
        return;
    }

    int dia, mes, ano;
    char nome_arquivo_saida[100];
    limpar_tela();
    printf("=========================================\n");
    printf("         Exportar Agenda do Dia          \n");
    printf("=========================================\n\n");
    printf("Digite o dia: ");
    scanf(" %d",&dia);
    printf("Digite o mes: ");
    scanf(" %d",&mes);
    printf("Digite o ano (4 digitos): ");
    scanf(" %d",&ano);
    
    Data data_busca = criar_data(dia,mes,ano);
    if (!validar_data(data_busca)) {
        exibir_mensagem("Erro: Data informada invalida!");
        pausar_tela();
        return;
    }
    // formatação segura do nome do arquivo
    sprintf(nome_arquivo_saida,"agenda_%04d-%02d-%02d.txt",data_busca.ano,data_busca.mes,data_busca.dia);
    const char data_busca_string[11];
    sprintf(data_busca_string,"%02d/%02d/%04d",data_busca.dia,data_busca.mes,data_busca.ano);
    printf("\nGerando relatorio em: %s ...\n",nome_arquivo_saida);
    int sucesso = exportar_agenda_dia_txt(lista,data_busca_string,nome_arquivo_saida);
    if (sucesso) {
        exibir_mensagem("Agenda exportada com sucesso!");
    } else {
        exibir_mensagem("Erro ao criar ou gravar o arquivo exportado.");
    }
    pausar_tela();
}

void limpar_tela(void){
    #ifdef _WIN32       // diretivas do pré-processador ("se esse sistema for Windows")
        system("cls");  // comando enviado diretamente para o terminal do sistema operacional
    #else               // caso contrário, assume que é Linux, macOS ou outro sistema compatível
        system("clear");
    #endif
}

// Pausar a tela antes de voltar ao menu principal
void pausar_tela(void){
    printf("\nPressione ENTER para continuar...");
    getchar(); // captura o ENTER do scanf anterior
    getchar(); // aguarda o novo ENTER do usuário
}

void exibir_mensagem(const char* mensagem){
    printf("\n>>> %s <<<\n", mensagem);
}