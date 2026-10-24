# Gerenciador de Tarefas com Subtarefas 

**Disciplina:** Algoritmos e Programação de Computadores II  
**Tema T5:** Gerenciador de Tarefas com Subtarefas  


---

##  Integrantes 

* **Giovana Oliveira Geraldo**
* **Izabela da Silva Lopes** 
* **Kauanny Vitória de Souza Fernandes**
* **Letícia Gomes Santos**
* **Letícia Gonçalves Ferreira**

---

## Sobre o Projeto

O **Gerenciador de Tarefas com Subtarefas** é um sistema desenvolvido em C focado no gerenciamento hierárquico de tarefas principais e suas respectivas sub-ações (subtarefas). 

### Principais Características
* **Hierarquia Pai-Filho:** Cada tarefa principal comporta até 10 subtarefas vinculadas (`MAX_SUBTAREFAS = 10`).
* **Progresso Dinâmico e Recursivo:** O percentual de conclusão da tarefa pai não é inserido pelo usuário; ele é calculado de forma recursiva com base no status de conclusão de suas subtarefas.
* **Persistência de Dados:** Salvamento/carregamento dos dados em arquivo binário (`dados.bin`) e geração de relatórios/agendas organizadas em arquivos de texto (`.txt`).
* **Filtros e Ordenação:** Classificação por níveis de prioridade (*Baixa, Média, Alta*) e prazos limites (*Data*).

---

##  Estrutura do Repositório

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

