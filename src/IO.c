#include "../headers/IO.h"
#include "../headers/avl.h"
#include "../headers/fila.h"
#include "../headers/paciente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Função auxiliar para escapar string JSON (adiciona escape para caracteres especiais)
static void json_escape_string(const char* str, FILE* fp) {
    if (str == NULL) {
        fputs("\"\"", fp);
        return;
    }
    
    fputc('"', fp);
    for (const char* p = str; *p != '\0'; p++) {
        switch (*p) {
            case '"':  fputs("\\\"", fp); break;
            case '\\': fputs("\\\\", fp); break;
            case '\n': fputs("\\n", fp); break;
            case '\r': fputs("\\r", fp); break;
            case '\t': fputs("\\t", fp); break;
            default:
                if (*p >= 0 && *p < 32) {
                    // Caracteres de controle - escapa como unicode
                    fprintf(fp, "\\u%04x", (unsigned char)*p);
                } else {
                    fputc(*p, fp);
                }
                break;
        }
    }
    fputc('"', fp);
}

bool io_salvar(AVL* lista_pacientes, FILA* fila_espera) {
    if (lista_pacientes == NULL || fila_espera == NULL) {
        return false;
    }
    
    // Salvar pacientes da AVL em JSON
    FILE* fp_pacientes = fopen("pacientes.json", "w");
    if (fp_pacientes == NULL) {
        return false;
    }
    
    // Obtém todos os pacientes da AVL
    int tamanho_pacientes;
    PACIENTE** pacientes = lista_obter_todos_pacientes(lista_pacientes, &tamanho_pacientes);
    
    fprintf(fp_pacientes, "{\n  \"pacientes\": [\n");
    
    if (pacientes != NULL && tamanho_pacientes > 0) {
        // Salva cada paciente
        for (int i = 0; i < tamanho_pacientes; i++) {
            if (pacientes[i] != NULL) {
                int id = paciente_get_id(pacientes[i]);
                const char* nome = paciente_get_name(pacientes[i]);
                int prioridade = paciente_get_prioridade(pacientes[i]);
                
                fprintf(fp_pacientes, "    {\n");
                fprintf(fp_pacientes, "      \"id\": %d,\n", id);
                fprintf(fp_pacientes, "      \"nome\": ");
                json_escape_string(nome, fp_pacientes);
                fprintf(fp_pacientes, ",\n");
                fprintf(fp_pacientes, "      \"prioridade\": %d\n", prioridade);
                
                if (i < tamanho_pacientes - 1) {
                    fprintf(fp_pacientes, "    },\n");
                } else {
                    fprintf(fp_pacientes, "    }\n");
                }
            }
        }
    }
    
    fprintf(fp_pacientes, "  ]\n}");
    fclose(fp_pacientes);
    
    if (pacientes != NULL) {
        free(pacientes); // Libera o array, mas não os pacientes
    }
    
    // Salvar fila de espera em JSON (organizada por prioridade)
    FILE* fp_fila = fopen("fila_espera.json", "w");
    if (fp_fila == NULL) {
        return false;
    }
    
    fprintf(fp_fila, "{\n");
    
    const char* nomes_prioridade[] = {
        "nao_urgente",
        "pouco_urgente",
        "urgente",
        "muito_urgente",
        "emergencia"
    };
    
    bool primeiro_campo = true;
    
    // Salva cada prioridade como um array
    for (int prioridade = 0; prioridade < 5; prioridade++) {
        int tamanho_ids;
        int* ids = fila_obter_ids_por_prioridade(fila_espera, prioridade, &tamanho_ids);
        
        if (!primeiro_campo) {
            fprintf(fp_fila, ",\n");
        }
        primeiro_campo = false;
        
        fprintf(fp_fila, "  \"%s\": [", nomes_prioridade[prioridade]);
        
        if (ids != NULL && tamanho_ids > 0) {
            for (int i = 0; i < tamanho_ids; i++) {
                fprintf(fp_fila, "%d", ids[i]);
                if (i < tamanho_ids - 1) {
                    fprintf(fp_fila, ", ");
                }
            }
            free(ids);
        }
        
        fprintf(fp_fila, "]");
    }
    
    fprintf(fp_fila, "\n}");
    fclose(fp_fila);
    
    return true;
}

// Função auxiliar para pular espaços em branco
static void pular_espacos(FILE* fp) {
    int c;
    while ((c = fgetc(fp)) != EOF && (c == ' ' || c == '\t' || c == '\n' || c == '\r')) {
        // Pula espaços
    }
    if (c != EOF) {
        ungetc(c, fp);
    }
}

// Função auxiliar para ler string JSON
static bool ler_string_json(FILE* fp, char* buffer, int max_len) {
    pular_espacos(fp);
    int c = fgetc(fp);
    if (c != '"') {
        ungetc(c, fp);
        return false;
    }
    
    int i = 0;
    bool escape = false;
    while (i < max_len - 1) {
        c = fgetc(fp);
        if (c == EOF) return false;
        
        if (escape) {
            switch (c) {
                case 'n': buffer[i++] = '\n'; break;
                case 'r': buffer[i++] = '\r'; break;
                case 't': buffer[i++] = '\t'; break;
                case '"': buffer[i++] = '"'; break;
                case '\\': buffer[i++] = '\\'; break;
                default: buffer[i++] = c; break;
            }
            escape = false;
        } else if (c == '\\') {
            escape = true;
        } else if (c == '"') {
            break;
        } else {
            buffer[i++] = c;
        }
    }
    
    buffer[i] = '\0';
    return true;
}

// Função auxiliar para ler número inteiro
static bool ler_numero_json(FILE* fp, int* valor) {
    pular_espacos(fp);
    return (fscanf(fp, "%d", valor) == 1);
}

bool io_carregar(AVL** lista_pacientes, FILA** fila_espera) {
    if (lista_pacientes == NULL || fila_espera == NULL) {
        return false;
    }
    
    // Carregar pacientes da AVL
    FILE* fp_pacientes = fopen("pacientes.json", "r");
    if (fp_pacientes == NULL) {
        // Arquivo não existe, não é erro - pode ser primeira execução
        return true;
    }
    
    pular_espacos(fp_pacientes);
    int c = fgetc(fp_pacientes);
    if (c != '{') {
        fclose(fp_pacientes);
        return false;
    }
    
    // Procura por "pacientes"
    char buffer[256];
    while (fscanf(fp_pacientes, "%255s", buffer) == 1) {
        if (strcmp(buffer, "\"pacientes\"") == 0) {
            pular_espacos(fp_pacientes);
            c = fgetc(fp_pacientes);
            if (c != ':') {
                fclose(fp_pacientes);
                return false;
            }
            pular_espacos(fp_pacientes);
            c = fgetc(fp_pacientes);
            if (c != '[') {
                fclose(fp_pacientes);
                return false;
            }
            break;
        }
    }
    
    // Lê array de pacientes
    pular_espacos(fp_pacientes);
    c = fgetc(fp_pacientes);
    
    while (c != ']' && c != EOF) {
        if (c == '{') {
            int id = -1, prioridade = -1;
            char nome[100] = {0};
            
            // Lê campos do paciente
            while (1) {
                pular_espacos(fp_pacientes);
                if (fscanf(fp_pacientes, "%255s", buffer) != 1) break;
                
                if (strcmp(buffer, "\"id\"") == 0) {
                    pular_espacos(fp_pacientes);
                    fgetc(fp_pacientes); // ':'
                    ler_numero_json(fp_pacientes, &id);
                } else if (strcmp(buffer, "\"nome\"") == 0) {
                    pular_espacos(fp_pacientes);
                    fgetc(fp_pacientes); // ':'
                    ler_string_json(fp_pacientes, nome, sizeof(nome));
                } else if (strcmp(buffer, "\"prioridade\"") == 0) {
                    pular_espacos(fp_pacientes);
                    fgetc(fp_pacientes); // ':'
                    ler_numero_json(fp_pacientes, &prioridade);
                }
                
                pular_espacos(fp_pacientes);
                c = fgetc(fp_pacientes);
                if (c == '}') break;
                if (c != ',') {
                    ungetc(c, fp_pacientes);
                    break;
                }
            }
            
            // Cria paciente e insere na AVL
            if (id >= 0 && prioridade >= 0 && prioridade <= 4 && nome[0] != '\0') {
                PACIENTE* paciente = paciente_criar(id, nome, prioridade);
                if (paciente != NULL) {
                    lista_inserir_paciente(*lista_pacientes, paciente);
                }
            }
        }
        
        pular_espacos(fp_pacientes);
        c = fgetc(fp_pacientes);
        if (c == ',') {
            pular_espacos(fp_pacientes);
            c = fgetc(fp_pacientes);
        }
    }
    
    fclose(fp_pacientes);
    
    // Carregar fila de espera
    FILE* fp_fila = fopen("fila_espera.json", "r");
    if (fp_fila == NULL) {
        // Arquivo não existe, não é erro - fila pode estar vazia
        return true;
    }
    
    pular_espacos(fp_fila);
    c = fgetc(fp_fila);
    if (c != '{') {
        fclose(fp_fila);
        return false;
    }
    
    const char* nomes_prioridade[] = {
        "nao_urgente",
        "pouco_urgente",
        "urgente",
        "muito_urgente",
        "emergencia"
    };
    
    // Lê cada campo de prioridade
    for (int prioridade = 0; prioridade < 5; prioridade++) {
        // Procura pelo campo da prioridade
        rewind(fp_fila);
        pular_espacos(fp_fila);
        c = fgetc(fp_fila);
        if (c != '{') continue;
        
        char campo[256];
        snprintf(campo, sizeof(campo), "\"%s\"", nomes_prioridade[prioridade]);
        
        // Procura pelo campo
        while (fscanf(fp_fila, "%255s", buffer) == 1) {
            if (strcmp(buffer, campo) == 0) {
                pular_espacos(fp_fila);
                c = fgetc(fp_fila);
                if (c != ':') break;
                pular_espacos(fp_fila);
                c = fgetc(fp_fila);
                if (c != '[') break;
                
                // Lê array de IDs
                pular_espacos(fp_fila);
                c = fgetc(fp_fila);
                
                while (c != ']' && c != EOF) {
                    if ((c >= '0' && c <= '9') || c == '-') {
                        ungetc(c, fp_fila);
                        int id_fila;
                        if (ler_numero_json(fp_fila, &id_fila)) {
                            // Busca paciente na AVL pelo ID
                            PACIENTE* paciente = lista_buscar_paciente(*lista_pacientes, id_fila);
                            if (paciente != NULL) {
                                // Insere na fila
                                fila_inserir_paciente(*fila_espera, paciente);
                            }
                        }
                    }
                    
                    pular_espacos(fp_fila);
                    c = fgetc(fp_fila);
                    if (c == ',') {
                        pular_espacos(fp_fila);
                        c = fgetc(fp_fila);
                    }
                }
                break;
            }
        }
    }
    
    fclose(fp_fila);
    return true;
}
