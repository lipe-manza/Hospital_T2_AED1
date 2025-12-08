# Projeto 2 - Pronto Socorro SUS (V2)

**Disciplina:** SCC0202 - Algoritmos e Estruturas de Dados I  
**Professores:** Rudinei e JB  
**Monitores:** Laura e Felipe

---

## Integrantes

| Nome Completo | Número USP |
|---|---:|
| Luiz Felipe Manzoli Franceschini | 16913300 |
| Gustavo Vieira Gomes | 16907251 |
| Guilherme Pego dos Santos | 15575570 |

---

## Visão Geral

Este projeto implementa a simulação de um Pronto Socorro que atende pacientes conforme uma política baseada em níveis de prioridade, não apenas ordem de chegada. O sistema utiliza estruturas de dados eficientes como árvore AVL e filas encadeadas para gerenciar o fluxo de pacientes, com persistência de dados em arquivos binários.

---

## Níveis de Prioridade

0. Emergência
1. Muito urgente
2. Urgente
3. Pouco urgente
4. Não urgente

Pacientes com a mesma prioridade são atendidos respeitando a ordem de chegada (FIFO).

---

## Estruturas de Dados Utilizadas

### AVL - Lista de Todos os Pacientes

Armazena a totalidade dos pacientes cadastrados usando ID como chave única. Mantém os ponteiros reais dos pacientes que existem de forma única em memória.

### FILA - Fila de Espera com Prioridade

Implementada como cinco filas internas, uma para cada nível de prioridade. Cada fila interna é encadeada e preserva a ordem de chegada dos pacientes.

### PACIENTE

Tipo abstrato que representa um paciente com os campos: `int id` (identificador único), `char nome[100]` (nome do paciente) e `int prioridade` (nível de prioridade entre 1 e 5).

### OP

Módulo que implementa todas as operações disponíveis no menu do sistema como registrar, remover, buscar, imprimir fila e dar alta.

---

## Funcionalidades do Menu

O sistema oferece sete operações principais para gerenciamento de pacientes:

1. **Registrar paciente:** Insere um novo paciente no sistema e o adiciona automaticamente à fila de espera conforme seu nível de prioridade.

2. **Remover paciente:** Remove um paciente do sistema (só é possível se ele não estiver na fila de espera).

3. **Listar pacientes:** Exibe todos os pacientes cadastrados em ordem crescente de ID, conforme armazenados na árvore AVL.

4. **Buscar paciente por ID:** Localiza e exibe informações de um paciente específico mediante seu identificador.

5. **Mostrar fila de espera:** Exibe a fila ordenada por prioridade, mostrando primeiro os pacientes de maior prioridade (emergência) até os de menor prioridade (não urgente).

6. **Dar alta ao paciente:** Remove o primeiro paciente disponível da maior prioridade da fila de espera, mas não o remove da lista geral de pacientes.

7. **Sair:** Encerra o programa, salvando todos os dados em arquivos binários.

---

## Regras Importantes

- IDs não podem se repetir entre os pacientes cadastrados.
- Ao registrar um paciente, ele é inserido automaticamente na fila conforme seu nível de prioridade.
- Um paciente só pode ser removido do sistema se não estiver aguardando na fila de espera.
- Ao dar alta, remove-se o primeiro paciente da maior prioridade disponível naquele momento.
- A árvore AVL sempre contém a totalidade dos pacientes cadastrados, inclusive aqueles que já foram atendidos.

---

## Persistência de Dados

O sistema utiliza dois arquivos binários para armazenar dados: `pacientes.bin` e `fila.bin`. Nenhuma informação é armazenada em formato JSON ou texto simples.

### pacientes.bin

Armazena todos os pacientes cadastrados. Formato: inteiro indicando o total de pacientes seguido pela sequência de estruturas PACIENTE lidas da árvore AVL em ordem crescente de ID.

### fila.bin

Armazena as filas de espera separadas por nível de prioridade. Formato: para cada prioridade de 0 a 4, armazena um inteiro indicando a quantidade de IDs seguido pela sequência de identificadores preservando a ordem exata da fila.

Nenhum ponteiro é gravado nos arquivos. A fila é reconstruída em memória criando novos nós, mas reutilizando os ponteiros reais dos pacientes encontrados na árvore AVL.

---

## Fluxo do Programa

Na inicialização, o programa cria estruturas vazias de AVL e FILA e tenta carregar dados dos arquivos binários. Se o carregamento falhar, as estruturas permanecem vazias e o usuário inicia com um sistema limpo. Durante a execução, o usuário interage com um menu que chama funções do módulo OP. Ao escolher a opção Sair, todos os dados são salvos em arquivos binários antes de liberar memória e encerrar.

---

## Compilação e Execução

Utilize o Makefile para compilar e executar o projeto:

```bash
make              # Compila o projeto gerando o executável
make run          # Compila e executa o programa diretamente
make clean        # Remove arquivos objeto (.o) e o executável
make cleanall     # Remove arquivos objeto, executável e os arquivos binários (.bin)
```

---

## Considerações de Implementação

O módulo `io_carregar` constrói estruturas de dados temporárias e só substitui as originais se o carregamento for bem-sucedido, evitando vazamentos de memória e corrupções de dados. O programa utiliza gerenciamento dinâmico de memória com liberação apropriada ao finalizar. A estrutura de árvore AVL garante busca eficiente em O(log n) e a implementação de filas encadeadas permite inserção e remoção em O(1) na fila de espera.