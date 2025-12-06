#include <stdlib.h>
#include <stdio.h>
#include "avl.h"
#define max(a,b) ((a>b)? a: b)

// Estrutura do nó (usa o TAD PACIENTE)
struct no {
    PACIENTE* paciente;
    int height; // para calcular balance factor
    NO* left;
    NO* right;
};

// Estrutura da AVL
struct avl {
    NO* root;
};

// Cria uma árvore vazia
AVL* lista_criar(void) {
    AVL* t = (AVL*)malloc(sizeof(AVL));
    if (t != NULL) {
        t->root = NULL;
    }
    return t;
}

// Cria nó (privado)
static NO* avl_criar_no(PACIENTE* paciente) {
    NO* new_node = (NO*)malloc(sizeof(NO));
    if (new_node) {
        new_node->paciente = paciente; // guarda ponteiro (não duplica)
        new_node->height = 0;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

static bool lista_cheia() {
    NO* teste = malloc(sizeof(NO));
    if (teste == NULL)
        return true;
    free(teste);
    return false;
}


// Apaga recursivamente a árvore (libera pacientes também)
static void avl_apagar_aux(NO* root) {
    if (root != NULL) {
        avl_apagar_aux(root->left);
        avl_apagar_aux(root->right);
        paciente_apagar(&root->paciente); // libera PACIENTE via TAD
        free(root);
    }
}

// API pública: apaga a árvore e seta ponteiro NULL
void lista_apagar(AVL** T) {
    if (T && *T) {
        avl_apagar_aux((*T)->root);
        free(*T);
        *T = NULL;
    }
}

// Busca recursiva (privada)
static PACIENTE* avl_busca_aux(NO* root, int id) {
    if (root == NULL) return NULL;
    if (id == paciente_get_id(root->paciente))
        return root->paciente;
    if (id < paciente_get_id(root->paciente))
        return avl_busca_aux(root->left, id);
    else
        return avl_busca_aux(root->right, id);
}

// API pública de busca (usa nó raiz)
PACIENTE* lista_buscar_paciente(AVL* T, int id) {
    if (T == NULL) return NULL;
    return avl_busca_aux(T->root, id);
}

// Retorna altura do nó (-1 se NULL)
static int avl_altura_no(NO* root) {
    return (root == NULL) ? -1 : root->height;
}

// Rotações (privadas)
static NO* rotacao_direita(NO* A) {
    NO* B = A->left;
    A->left = B->right;
    B->right = A;

    A->height = max(avl_altura_no(A->left), avl_altura_no(A->right)) + 1;
    B->height = max(avl_altura_no(B->left), A->height) + 1;
    return B;
}

static NO* rotacao_esquerda(NO* A) {
    NO* B = A->right;
    A->right = B->left;
    B->left = A;

    A->height = max(avl_altura_no(A->left), avl_altura_no(A->right)) + 1;
    B->height = max(avl_altura_no(B->left), A->height) + 1;
    return B;
}

static NO* rotacao_esquerda_direita(NO* A) {
    A->left = rotacao_esquerda(A->left);
    return rotacao_direita(A);
}

static NO* rotacao_direita_esquerda(NO* A) {
    A->right = rotacao_direita(A->right);
    return rotacao_esquerda(A);
}

// Inserção recursiva (recebe nó pré-criado)
static NO* avl_inserir_no_aux(NO* root, NO* new_node) {
    if (root == NULL) {
        root = new_node; // insere aqui
    }
    else if (paciente_get_id(new_node->paciente) < paciente_get_id(root->paciente)) {
        root->left = avl_inserir_no_aux(root->left, new_node);
    }
    else if (paciente_get_id(new_node->paciente) > paciente_get_id(root->paciente)) {
        root->right = avl_inserir_no_aux(root->right, new_node);
    }
    else {
        // chave igual: não insere duplicata (novo nó deve ser liberado pelo chamador,
        // mas neste design controlamos antes de criar o nó, então aqui não deve ocorrer)
        return root;
    }

    // atualiza altura e reequilibra
    root->height = max(avl_altura_no(root->left), avl_altura_no(root->right)) + 1;
    int BF = avl_altura_no(root->left) - avl_altura_no(root->right);

    if (BF == -2) {
        if (avl_altura_no(root->right->left) - avl_altura_no(root->right->right) <= 0)
            root = rotacao_esquerda(root);
        else
            root = rotacao_direita_esquerda(root);
    }
    else if (BF == 2) {
        if (avl_altura_no(root->left->left) - avl_altura_no(root->left->right) >= 0)
            root = rotacao_direita(root);
        else
            root = rotacao_esquerda_direita(root);
    }

    return root;
}

// API pública: insere paciente
bool lista_inserir_paciente(AVL* T, PACIENTE* paciente) {
    if (T == NULL || paciente == NULL) return false;

    // evita duplicata: se já existir paciente com mesmo id, não inserimos
    if (lista_buscar_paciente(T, paciente_get_id(paciente)) != NULL) {
        return false;
    }

    if (lista_cheia()) {
        printf("Erro: memória insuficiente para inserir novo paciente na árvore.\n");
        return false;
    }
    NO* new_node = avl_criar_no(paciente);
    if (new_node == NULL) return false;

    T->root = avl_inserir_no_aux(T->root, new_node);
    if (T->root != NULL)
        return true;

    printf("Erro ao inserir paciente na árvore.\n");
    return false;
}


// Helper para remover: procura maior na subárvore esquerda e o coloca no nó r
static void swap_left_max(NO* t, NO* r, NO* a) {
    if (t->right != NULL) {
        swap_left_max(t->right, r, t);
        return;
    }

    // t é o maior da subárvore esquerda
    if (r == a) // caso especial: o primeiro da esquerda já é o maior
        a->left = t->left;
    else
        a->right = t->left;

    // substitui o paciente de r pelo de t e libera o paciente antigo de r
    PACIENTE* p_antigo = r->paciente;
    r->paciente = t->paciente;        // move paciente do nó t para r
    paciente_apagar(&p_antigo);       // libera o antigo paciente de r
    free(t);                          // libera o nó t (não libera o PACIENTE movido)
    t = NULL;
}

// Remoção recursiva por id
static NO* avl_remover_aux(NO* root, int id) {
    NO* p;
    if (root == NULL) return NULL;

    if (id == paciente_get_id(root->paciente)) {
        // Caso 1/2: 0 ou 1 filho
        if (root->left == NULL || root->right == NULL) {
            p = root;
            if (root->left == NULL)
                root = root->right;
            else
                root = root->left;
            paciente_apagar(&p->paciente); // libera PACIENTE do nó removido
            free(p);
            p = NULL;
        }
        else {
            // Caso 3: dois filhos -> substitui pelo maior da esquerda
            swap_left_max(root->left, root, root);
        }
    }
    else if (id < paciente_get_id(root->paciente)) {
        root->left = avl_remover_aux(root->left, id);
    }
    else {
        root->right = avl_remover_aux(root->right, id);
    }

    // reequilibra se root ainda existe
    if (root != NULL) {
        root->height = max(avl_altura_no(root->left), avl_altura_no(root->right)) + 1;
        int BF = avl_altura_no(root->left) - avl_altura_no(root->right);

        if (BF == -2) {
            if (avl_altura_no(root->right->left) - avl_altura_no(root->right->right) <= 0)
                root = rotacao_esquerda(root);
            else
                root = rotacao_direita_esquerda(root);
        }
        else if (BF == 2) {
            if (avl_altura_no(root->left->left) - avl_altura_no(root->left->right) >= 0)
                root = rotacao_direita(root);
            else
                root = rotacao_esquerda_direita(root);
        }
    }

    return root;
}

// API pública: remove paciente por id
bool lista_remover_paciente(AVL* T, int id) {
    if (T == NULL) return false;

    // verifica se existe antes de remover
    if (lista_buscar_paciente(T, id) == NULL)
        return false;

    T->root = avl_remover_aux(T->root, id);
    return true;
}

// Impressão em ordem (in-order traversal)
static void lista_imprimir_aux(NO* root) {
    if (root != NULL) {
        lista_imprimir_aux(root->left);
        paciente_imprimir(root->paciente);
        lista_imprimir_aux(root->right);
    }
}

void lista_imprimir(AVL* T) {
    if (T == NULL) {
        printf("Lista nao existe.\n");
        return;
    }
    if (T->root == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    lista_imprimir_aux(T->root);
}

