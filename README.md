# Simulador de Escalonamento de Processos

Projeto em C++20 para simular algoritmos de escalonamento de processos. A
simulação pode ser executada pelo terminal ou acompanhada em uma interface
gráfica.

## Algoritmos disponíveis

- FCFS: executa primeiro o processo que chegou antes, sem preempção.
- SJF: escolhe o processo pronto com menor duração, sem preempção.
- SRTF: escolhe o processo com menor tempo restante e permite preempção.
- Prioridade cooperativa: escolhe a maior prioridade quando a CPU fica livre.
- Prioridade preemptiva: pode trocar o processo em execução por outro de maior
  prioridade.
- Round-Robin: alterna entre processos prontos usando um quantum.
- Round-Robin com prioridade e envelhecimento: usa quantum e aumenta a
  prioridade dinâmica dos processos que aguardam.

Em caso de empate, os escalonadores usam uma regra compartilhada: mantêm o
processo atual quando aplicável, preferem o menor tempo restante e, se ainda
houver empate, escolhem entre os candidatos empatados.

## Estrutura do projeto

- `core/`: modelo de processo, interface comum dos escalonadores e simulador.
- `schedulers/`: implementação dos algoritmos e criação do escalonador escolhido.
- `io/`: leitura de processos e configurações, além da formatação do relatório.
- `cli/`: versão para execução pelo terminal.
- `app/`: interface gráfica, com entrada de dados, simulação e resultados.

## Decisões de implementação

### Processo

Cada processo é representado pela estrutura `Processo`, em
`core/include/processo.hpp`. Ela reúne as informações necessárias para
identificar e acompanhar o processo:

| Campo | Descrição |
| --- | --- |
| `id` | Identificador do processo. |
| `tempo_chegada` | Instante em que o processo fica disponível. |
| `duracao` | Tempo total de CPU necessário. |
| `tempo_restante` | Tempo de CPU que falta executar. |
| `prioridade` | Prioridade informada para o processo. |
| `prioridade_dinamica` | Prioridade usada no envelhecimento. |
| `estado` | Estado atual: novo, pronto, executando ou finalizado. |
| `tempo_inicio` | Primeiro instante em que o processo executou. |
| `tempo_conclusao` | Instante em que o processo terminou. |
| `tempo_espera` | Tempo total aguardando pela CPU. |
| `tempo_retorno` | Tempo entre a chegada e a conclusão. |

Os tempos de início e conclusão começam com `-1` para indicar que ainda não
foram registrados. O estado do processo é representado por `EstadoProcesso`,
uma enumeração que evita usar valores numéricos sem significado.

### Simulação e estruturas de dados

A classe `Simulador`, em `core/`, controla o relógio e avança a execução um tick
por vez. Em cada tick, ela incorpora os processos que chegaram, chama o
escalonador, atualiza o processo em execução, registra o resultado e verifica
se algum processo terminou. O avanço incremental permite que a interface
gráfica mostre a simulação em andamento; o método `executar()` repete esse
avanço até a conclusão.

Os processos são mantidos em um `std::vector<Processo>`. A fila de prontos é um
`std::vector<Processo*>` que aponta para esses processos, evitando manter
cópias separadas do estado. O simulador também guarda um ponteiro para o
processo atual. Esses ponteiros são usados enquanto o vetor de processos não é
redimensionado durante a simulação.

O resultado, `ResultadoSimulacao`, guarda uma cópia dos processos, os registros
de cada tick, as trocas de contexto e os tempos médios de retorno e espera.
Quando a CPU está ociosa, o registro do tick usa `-1` como identificador do
processo em execução.

### Organização dos escalonadores

Os algoritmos implementam a interface `IEscalonador`, que define como escolher
o próximo processo. O simulador trabalha com essa interface, sem precisar
conhecer as regras específicas de cada algoritmo.

Esse formato aplica o padrão **Strategy**: cada algoritmo é uma estratégia
intercambiável de escolha. A função `criar_escalonador` centraliza a criação
das implementações e recebe as configurações necessárias, como quantum e
envelhecimento.

A lógica comum para filtrar candidatos e resolver empates está em
`schedulers/include/desempate.hpp`, evitando repetir essas regras em cada
algoritmo.

### Entrada e interface

O módulo `io/` lê processos no formato `chegada duracao prioridade` e
configurações no formato `chave:valor`. Erros de leitura são associados à linha
de origem para facilitar a correção dos dados.

A interface gráfica mantém seus dados em `EstadoApp`. Ela usa o mesmo leitor,
modelo de processo, fábrica de escalonadores e simulador da versão de terminal;
assim, as regras da simulação são compartilhadas entre as duas formas de uso.

## Compilação

É necessário ter CMake 3.16 ou superior e um compilador com suporte a C++20.
Para compilar somente a versão de terminal:

```sh
cmake -S . -B build -DBUILD_GUI=OFF
cmake --build build
```

Para compilar também a interface gráfica, habilite a opção `BUILD_GUI`:

```sh
cmake -S . -B build -DBUILD_GUI=ON
cmake --build build
```

A interface utiliza OpenGL, GLFW, Dear ImGui e tinyfiledialogs. O CMake procura
o GLFW no sistema e, se não encontrá-lo, baixa as dependências declaradas pelo
projeto usando FetchContent. A primeira configuração com a interface gráfica
habilitada requer acesso à internet para baixar dependências que não estejam
disponíveis localmente.

## Uso pelo terminal

O executável `escalonador_cli` lê os processos pela entrada padrão. Por padrão,
executa os sete algoritmos:

```sh
./build/cli/escalonador_cli < processos.txt
```

Para executar apenas um algoritmo, informe seu número de 1 a 7:

```sh
./build/cli/escalonador_cli --algoritmo 3 < processos.txt
```

Também é possível indicar um arquivo de configuração:

```sh
./build/cli/escalonador_cli --config config.txt --all < processos.txt
```

Formato de `processos.txt`:

```text
# chegada duracao prioridade
0 5 2
1 3 4
```

Cada linha válida contém três inteiros: instante de chegada, duração positiva e
prioridade positiva. Linhas em branco e linhas iniciadas por `#` são ignoradas.
Os identificadores são atribuídos na ordem de leitura.

Formato de `config.txt`:

```text
quantum:2
aging:1
```

Os valores padrão são `quantum:2` e `aging:1`. Quantum deve ser maior que zero;
aging não pode ser negativo.

## Uso da interface gráfica

Execute `./build/app/escalonador_gui`. A interface permite adicionar, remover
e editar processos, carregar processos e configurações de arquivos, escolher
um algoritmo, acompanhar a simulação pelo diagrama de Gantt e consultar os
resultados.
