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

##  Regras de Negócio e Estrutura de Dados

### Estruturas Principais 
### `tarefas.h`
* **Data:** Contém `dia`, `mes` e `ano`.
* **Prioridade:** Mapeamento em `1 (Baixa)`, `2 (Média)` e `3 (Alta)`.
* **Subtarefa:** Possui `id`, `descricao`, status `concluida` (0 ou 1) e `id_tarefa_pai`.
* **Tarefa:** Entidade principal contendo `id`, `descricao`, `prazo` (Data), `prioridade`, `percentual_conclusao`, vetor estático de `subtarefas` e contador `qtd_subtarefas`.

### `Menu.h`
**Menu Principal** (exibir_menu_principal / executar_opcao): Renderiza as opções centrais do sistema no terminal e direciona o fluxo de execução com base na escolha do usuário.   
**Cadastro e Gerenciamento de Tarefas** (exibir_menu_tarefas / menu_cadastrar_tarefa): Exibe as opções de gerenciamento de tarefas e coleta interativamente os dados necessários (descrição, prazo e prioridade) para criar uma nova tarefa.   
**Gerenciamento de Subtarefas** (exibir_menu_subtarefas): Interface dedicada para incluir, remover e atualizar o progresso das sub-ações vinculadas a uma tarefa pai.  
**Filtros e Ordenação**(exibir_filtro / menu_filtrar_tarefas): Interface para filtrar e reorganizar a exibição das tarefas por critérios específicos, como nível de prioridade ou data limite. 


### arquivos.h
