# Projeto 2 — Pronto Socorro SUS - V2

**Disciplina:** SCC0202 - Algoritmos e Estruturas de Dados I  
**Professores:** Rudinei e JB  
**Monitores:** Laura e Felipe  

---

## Integrantes

| Nome completo | Número USP |
|----------------|-------------|
| Luiz Felipe Manzoli Franceschini | 16913300 |
| Gustavo Vieira Gomes | 16907251 |
| Guilherme Pego dos Santos | 15575570 |

---

## Descrição do Projeto

Este projeto implementa a simulação de um **Pronto Socorro (PS)** seguindo a nova política nacional de atendimento, em que **a ordem de atendimento não é mais por chegada**, mas sim baseada em **5 níveis de prioridade**:

1. Emergência — risco imediato de morte  
2. Muito urgente — grave e pode evoluir para risco de morte  
3. Urgente — moderado, sem risco imediato  
4. Pouco urgente — poderia ser atendido em UBS  
5. Não urgente — casos leves, sem risco  

Pacientes com a mesma prioridade respeitam a **ordem de chegada**.

Para garantir eficiência e evitar operações lineares, o sistema utiliza **TADs** otimizados (lista, fila de prioridade, pilha) e realiza **persistência dos dados em disco** usando arquivos **JSON**, gravados ao sair e carregados ao iniciar.

---

## Estrutura do Sistema

- **Lista de Pacientes:** Armazena todos os pacientes cadastrados no hospital. Cada paciente possui nome e ID (único).  
- **Fila de Espera (TAD de prioridade):** Organiza os pacientes segundo a prioridade clínica, mantendo ordem de chegada dentro de prioridades iguais.  
- **Histórico de Atendimento (Pilha):** Armazena registros de atendimento realizados em cada paciente.  
- **Paciente (Item):** Estrutura que contém ID, nome e dados associados.  
- **Persistência (IO):** TAD que salva e carrega lista, fila e históricos em JSON.

---

## Funcionalidades do Menu Principal

1. Registrar paciente  
2. Remover paciente  
3. Listar pacientes  
4. Buscar paciente por ID  
5. Mostrar fila de espera  
6. Dar alta ao paciente  
7. Sair  

### Resumo das operações

- **Registrar paciente:** Cadastra um novo paciente (IDs repetidos não são permitidos) e o insere imediatamente na fila, respeitando a prioridade.  
- **Remover paciente:** Só é possível se o paciente **não estiver** na fila de espera.  
- **Listar pacientes:** Exibe todos os pacientes cadastrados.  
- **Buscar por ID:** Localiza um paciente diretamente pela chave.  
- **Mostrar fila:** Exibe pacientes da **maior para a menor prioridade**, respeitando ordem de chegada.  
- **Dar alta:** Paciente é removido da fila, mas permanece no registro histórico.  
- **Sair:** Dispara o salvamento completo dos dados em JSON.

---

## Persistência de Dados

Toda a persistência é feita em arquivos `.json`. Os dados são salvos **somente ao sair do programa** e recarregados automaticamente quando o programa inicia.

Arquivos utilizados:

- `lista_de_pacientes.json` — banco de pacientes  
- `fila_de_espera.json` — fila de triagem organizada por prioridade  
- `historicos.json` — histórico médico dos pacientes  

A persistência é implementada por um **TAD IO** ou por funções `save()` e `load()` adicionadas às estruturas (lista, fila, pilha).

---

## Compilação e Execução

O projeto utiliza um **Makefile** para compilar e executar todas as partes do sistema.

Comandos:

```bash
make          # Compila o projeto
make all      # Mesmo que make
make run      # Compila e executa
make clean    # Remove arquivos .o e o executável
make cleanall # Remove tudo, incluindo arquivos de dados JSON
make rebuild  # Recompila tudo do zero
make check    # Verifica arquivos fonte
make help     # Exibe a ajuda
