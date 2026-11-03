#ifndef ARQUIVOS_H
#define ARQUIVOS_H

#include "tarefas.h"
#define NOME_ARQUIVO "tarefas.txt"

// Protótipos das funções que manipulam arquivos
int salvar_tarefas_arquivo(const char* nome_arquivo, const Tarefa* lista);
int carregar_tarefas_arquivo(const char* nome_arquivo, Tarefa** lista_geral);

int exportar_agenda_dia_txt(const char* nome_arquivo, const Tarefa* lista, Data data_alvo);

#endif // ARQUIVOS_H