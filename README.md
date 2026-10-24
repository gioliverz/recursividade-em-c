# Gerenciador de Tarefas com Subtarefas 📝

**Disciplina:** Algoritmos e Programação de Computadores II  
**Tema T5:** Gerenciador de Tarefas com Subtarefas  
**Data:** 01/10/2026  

---

## 👥 Integrantes e RAs

* **Giovana Oliveira Geraldo** — RA: 308756
* **Izabela da Silva Lopes** — RA: 308773
* **Kauanny Vitória de Souza Fernandes** — RA: 308785
* **Letícia Gomes Santos** — RA: 308790
* **Letícia Gonçalves Ferreira** — RA: 308791

---

## 📌 Sobre o Projeto

O **Gerenciador de Tarefas com Subtarefas** é um sistema desenvolvido em C focado no gerenciamento hierárquico de tarefas principais e suas respectivas sub-ações (subtarefas). 

### Principais Características
* **Hierarquia Pai-Filho:** Cada tarefa principal comporta até 10 subtarefas vinculadas (`MAX_SUBTAREFAS = 10`).
* **Progresso Dinâmico e Recursivo:** O percentual de conclusão da tarefa pai não é inserido pelo usuário; ele é calculado de forma recursiva com base no status de conclusão de suas subtarefas.
* **Persistência de Dados:** Salvamento/carregamento dos dados em arquivo binário (`dados.bin`) e geração de relatórios/agendas organizadas em arquivos de texto (`.txt`).
* **Filtros e Ordenação:** Classificação por níveis de prioridade (*Baixa, Média, Alta*) e prazos limites (*Data*).

---

## 📁 Estrutura do Repositório

```text
recursividade-em-c/
├── include/            # Arquivos de cabeçalho (.h)
│   ├── arquivos.h      # Protótipos para leitura/escrita e relatórios
│   ├── menu.h          # Interface com o usuário e validações de input
│   └── tarefas.h       # Structs principais (Data, Subtarefa, Tarefa) e lógica de negócio
├── src/                # Arquivos de código fonte (.c)
│   ├── arquivos.c      # Implementação da persistência e geração de arquivos
│   ├── main.c          # Ponto de entrada do programa
│   ├── menu.c          # Implementação dos menus interativos
│   └── tarefas.c       # Implementação do CRUD de tarefas e funções recursivas
├── Makefile            # Script de automação de compilação
└── README.md           # Documentação do projeto
```

---

## 🛠️ Como Compilar e Executar

### Pré-requisitos
* Compilador **GCC** instalado.
* Utilitário **Make** (Linux/macOS) ou **MinGW Make** (Windows).

### Comandos do Makefile

* **Compilar o projeto:**
  ```bash
  make
  ```

* **Compilar e executar o programa diretamente:**
  ```bash
  make run
  ```

* **Limpar arquivos objeto e executáveis gerados:**
  ```bash
  make clean
  ```

* **Recompilar totalmente do zero:**
  ```bash
  make rebuild
  ```

---

## ⚙️ Regras de Negócio e Estrutura de Dados

### Estruturas Principais (`tarefas.h`)
* **Data:** Contém `dia`, `mes` e `ano`.
* **Prioridade:** Mapeamento em `1 (Baixa)`, `2 (Média)` e `3 (Alta)`.
* **Subtarefa:** Possui `id`, `descricao`, status `concluida` (0 ou 1) e `id_tarefa_pai`.
* **Tarefa:** Entidade principal contendo `id`, `descricao`, `prazo` (Data), `prioridade`, `percentual_conclusao`, vetor estático de `subtarefas` e contador `qtd_subtarefas`.

