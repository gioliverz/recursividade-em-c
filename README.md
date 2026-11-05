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


## Estruturas Principais 


### `arquivos.h`

* **Estruturas de Serialização** (`RegistroTarefaDisco` / `RegistroSubtarefaDisco`): Define structs estáticas sem ponteiros com empacotamento (`#pragma pack`), garantindo a serialização segura e determinística das tarefas e subtarefas em arquivo binário.
* **Persistência em Arquivo Binário** (`guardar_tarefas_binario` / `carregar_tarefas_binario` / `verificar_existencia_arquivo`): Salva a estrutura inteira de tarefas no disco em formato binário, restaura os dados para a memória RAM e verifica a existência prévia do arquivo.
* **Exportação de Relatórios em Texto** (`exportar_agenda_dia_txt` / `exportar_relatorio_geral_txt`): Gera relatórios legíveis em formato `.txt`, permitindo a exportação de um resumo geral de todas as tarefas ou de uma agenda filtrada por data específica.
* **Manipulação e Sanitização de Nomes** (`formatar_nome_arquivo_agenda`): Padroniza e trata a formatação dos nomes dos arquivos de relatórios, garantindo que as datas informadas gerem nomes de arquivo válidos no sistema operacional.

### `tarefas.h`

* **Estruturas de Dados e Constantes** (`Data` / `Subtarefa` / `Tarefa` / `MAX_SUBTAREFAS` / `PRIORIDADE_*`): Define limites operacionais, níveis de prioridade e os tipos estruturados principais organizados em listas encadeadas com suporte a ponteiros dinâmicos.
* **Manipulação e Validação de Datas** (`criar_data` / `validar_data` / `parse_data` / `comparar_datas` / `formatar_data`): Funções auxiliares para construção, validação no calendário, conversão, comparação e exibição de prazos limite.
* **Gerenciamento de Tarefas** (`criar_tarefa` / `remover_tarefa` / `editar_tarefa` / `listar_tarefas`): Controla o ciclo de vida das tarefas principais, incluindo alocação, exclusão de nós, edição de atributos e exibição da lista global.
* **Gerenciamento de Subtarefas** (`criar_sub` / `remover_sub` / `editar_sub` / `listar_sub`): Responsável por criar, remover, atualizar status/descrição e listar as sub-ações vinculadas à tarefa pai.
* **Filtros, Busca e Ordenação** (`busca_ID` / `ordenacao_por_prioridade` / `ordenacao_por_prazo`): Localiza tarefas por ID único global e reordena a lista encadeada por nível de prioridade ou prazo.
* **Cálculo Recursivo de Progresso** (`calculo_progresso` / `atualizaConclusao`): Percorre recursivamente as subtarefas vinculadas para calcular e atualizar o percentual de conclusão da tarefa principal.
* **Gestão de Memória Dinâmica** (`liberar_subtarefas` / `liberar_todas_tarefas`): Desaloca a memória dinâmica (`free`) das listas encadeadas, prevenindo *memory leaks* durante a execução do sistema.


### `menu.h`
* **Navegação e Menus Principais** (`exibir_menu_principal` / `exibir_menu_tarefas` / `exibir_menu_subtarefas` / `exibir_filtro` / `executar_opcao_principal`): Renderiza a interface no terminal, exibe as opções de menu e direciona o fluxo do sistema conforme a escolha do usuário.
* **Formulários de Entrada de Dados** (`menu_cadastrar_tarefa` / `menu_adicionar_subtarefa` / `menu_atualizar_progresso_subtarefa` / `menu_exportar_agenda`): Coleta interativamente os dados do usuário para cadastro de tarefas, inclusão de sub-ações, atualização de status e exportação.
* **Exibição e Formatação de Dados** (`mostrar_lista_tarefas` / `mostrar_detalhes_tarefa` / `exibir_mensagem`): Apresenta as listas de tarefas, detalha informações de um item específico e exibe mensagens de feedback ou alerta.
* **Gerenciamento da Interface** (`limpar_tela` / `pausar_tela`): Controla a exibição visual limpando o console e pausando a tela até a interação do usuário.
* **Leitura e Validação de Entrada** (`ler_string_segura` / `ler_inteiro`): Captura entradas do teclado, limpa o buffer de entrada e valida os tipos digitados para evitar erros no programa.


## Como Compilar e Executar

### Pré-requisitos
* Compilador *GCC* instalado.
* Utilitário *Make* (se for usar comandos "make").

---

###  Compilação

### Opção 1: Usando algum terminal

No powershell, git bash, cmd ou qualquer outro terminal execute: 
` gcc -Wall -Wextra -std=c99 -pedantic -g -Iinclude src/*.c -o gerenciador_tarefas `
`./gerenciador_tarefas`

#### Opção 2: Usando o Makefile 

Antes de executar esses comandos "make", certifique-se de estar 
na pasta correta. Exemplo: no terminal msys2, digite:

cd /c/Users/SEU-USUÁRIO/Desktop/GerenciadorTarefasC

execute `make run` para compilar e rodar
execute o comando `make` só para compilar
execute `make clean` para limpar/excluir os arquivos gerados

##  Instruções de Uso

#### 1. Navegação pelo Menu:
   - Utilize o menu interativo no terminal digitando o número correspondente à opção desejada.

#### 2. Gerenciamento de Tarefas:
   - Cadastre uma tarefa informando `Descrição`, `Prazo` (Dia/Mês/Ano) e nível de `Prioridade` (1 - Baixa, 2 - Média, 3 - Alta).
   - Visualize e organize a lista de tarefas aplicando filtros por nível de prioridade ou por data limite.

#### 3. Gerenciamento de Subtarefas:
   - Vincule até 10 subtarefas a uma tarefa pai existente.
   - Atualize o status de conclusão de uma subtarefa para que o percentual de progresso da tarefa pai seja calculado e atualizado automaticamente de forma recursiva.

#### 4. Persistência de Dados e Exportação:
   - Salve suas tarefas para mantê-las registradas no arquivo de dados.
   - Utilize a opção de carregar ao iniciar o sistema para restaurar as tarefas salvas anteriormente.
   - Exporte a agenda de um dia específico para gerar um relatório formatado em arquivo de texto (.txt).
