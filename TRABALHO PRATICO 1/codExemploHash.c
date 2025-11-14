#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 13               // tamanho da tabela (primo ajuda a dispersar melhor)
#define MAX_NOME 64

// Nó da lista (encadeamento separado)
typedef struct No {
    int ra;                 // chave
    char nome[MAX_NOME];    // valor
    struct No* prox;
} No;

// A tabela é um vetor de ponteiros para No
No* tabela[M];

// Função de hash (método da divisão)
int hash(int ra) {
    return ra % M;
}

// Inicializa todos os "baldes" com NULL
void inicializar() {
    for (int i = 0; i < M; i++) {
        tabela[i] = NULL;
    }
}

// Cria e retorna um novo nó
No* novo_no(int ra, const char* nome) {
    No* n = (No*) malloc(sizeof(No));
    n->ra = ra;
    strncpy(n->nome, nome, MAX_NOME - 1);
    n->nome[MAX_NOME - 1] = '\0';
    n->prox = NULL;
    return n;
}

// Insere (se RA já existir, atualiza o nome)
void inserir(int ra, const char* nome) {
    int idx = hash(ra);
    No* atual = tabela[idx];

    while (atual != NULL) {
        if (atual->ra == ra) {
            strncpy(atual->nome, nome, MAX_NOME - 1);
            atual->nome[MAX_NOME - 1] = '\0';
            return;
        }
        atual = atual->prox;
    }

    No* n = novo_no(ra, nome);
    n->prox = tabela[idx];
    tabela[idx] = n;
}

// Busca por RA; retorna ponteiro para nome ou NULL
const char* buscar(int ra) {
    int idx = hash(ra);
    No* atual = tabela[idx];

    while (atual != NULL) {
        if (atual->ra == ra) {
            return atual->nome;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Imprime a tabela hash
void imprimir() {
    printf("\n--- ESTADO DA TABELA HASH ---\n");
    for (int i = 0; i < M; i++) {
        printf("[%2d]: ", i);
        No* atual = tabela[i];
        if (!atual) {
            printf("(vazio)");
        }
        while (atual) {
            printf(" -> (RA:%d, Nome:%s)", atual->ra, atual->nome);
            atual = atual->prox;
        }
        printf("\n");
    }
}

// Libera memória
void destruir() {
    for (int i = 0; i < M; i++) {
        No* atual = tabela[i];
        while (atual) {
            No* prox = atual->prox;
            free(atual);
            atual = prox;
        }
        tabela[i] = NULL;
    }
}

// Calcula fator de carga
double fator_de_carga() {
    int n = 0;
    for (int i = 0; i < M; i++) {
        No* atual = tabela[i];
        while (atual) {
            n++;
            atual = atual->prox;
        }
    }
    return (double)n / (double)M;
}

int main() {
    inicializar();

    inserir(12345, "Ana");
    inserir(67890, "Bruno");
    inserir(11113, "Carla");
    inserir(22226, "Daniel");
    inserir(33339, "Ellen");
    inserir(44442, "Felipe");
    inserir(26, "Gabi");
    inserir(39, "Heitor");

    imprimir();

    int consultas[] = {12345, 39, 77777};
    for (int i = 0; i < 3; i++) {
        const char* achou = buscar(consultas[i]);
        if (achou) {
            printf("BUSCA: RA %d -> %s\n", consultas[i], achou);
        } else {
            printf("BUSCA: RA %d -> NAO ENCONTRADO\n", consultas[i]);
        }
    }

    printf("\nFator de carga aproximado: %.2f\n", fator_de_carga());

    destruir();
    return 0;
}