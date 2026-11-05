/**
 * ============================================================================
 * PROJETO PRÁTICO INTEGRADOR — TEMA T5: Gerenciador de Tarefas com Subtarefas
 * Diretório: recursividade-em-c/include/arquivos.h
 * Módulo 3: Persistência de Dados (Binária e Relatórios Textuais)
 * ============================================================================
 */

#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "tarefas.h"

#define MAGIC_HEADER "TSK5"
#define VERSAO_ARQUIVO 1

/**
 * Estruturas isoladas para serialização binária em disco.
 * 
 * JUSTIFICATIVA TÉCNICA E CRÍTICA AO ESCOPO INICIAL:
 * As structs dinâmicas em memória possuem ponteiros ('Subtarefa* subtarefas' e 'Tarefa* proxima').
 * Escrever diretamente 'sizeof(Tarefa)' com fwrite salvaria endereços efêmeros de memória RAM,
 * ignorando a lista de subtarefas e gerando falha de segmentação (Segmentation Fault)
 * ao recarregar os dados. Estas structs de disco garantem serialização determinística e segura.
 */
#pragma pack(push, 1)
typedef struct {
    int id;
    char descricao[101];
    Data prazo;
    int prioridade;
    float percentual_conclusao;
    int qtd_subtarefas;
} RegistroTarefaDisco;

typedef struct {
    int id;
    char descricao[101];
    int concluida;
    int id_tarefa_pai;
} RegistroSubtarefaDisco;
#pragma pack(pop)

/* Persistência em Arquivo Binário */
int guardar_tarefas_binario(const Tarefa* lista, const char* nome_arquivo);
Tarefa* carregar_tarefas_binario(const char* nome_arquivo);
int verificar_existencia_arquivo(const char* nome_arquivo);

/* Exportação de Relatórios em Texto */
int exportar_agenda_dia_txt(const Tarefa* lista, const char* data, const char* nome_arquivo_saida);
int exportar_relatorio_geral_txt(const Tarefa* lista, const char* nome_arquivo_saida);

/* Manipulação e Sanitização de Nomes de Arquivo */
void formatar_nome_arquivo_agenda(const char* data_entrada, char* buffer, int tamanho_buffer);

#endif /* ARQUIVOS_H */
