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

