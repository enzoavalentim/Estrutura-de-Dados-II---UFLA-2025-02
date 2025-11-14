#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>


#define M 101              
#define maxNome 60
#define maxCod 20
int selectFuncaoHash = 0;

typedef struct Produto {
    char cod[maxCod];                
    char nome[maxNome]; 
    int quantidade;
    double preco;
    struct No* prox;
} Produto;

Produto* criarNovoProduto (const char* cod, const char* nome, int quantidade, double preco) {

    Produto* p = (Produto*) malloc(sizeof(Produto));
    if (p == NULL) {
        return NULL;
    }

    strncpy(p->cod, cod, maxCod - 1);
    p->cod[maxCod - 1] = '\0';
    strncpy(p->nome, nome, maxNome - 1);
    p->nome[maxNome - 1] = '\0';
    p->quantidade = quantidade;
    p->preco = preco;
    p->prox = NULL;

    return p;
}

int main() {
    
    FILE *arquivo = fopen("produtosTechParts.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        exit(-1);
    }

    char linha[256];

    // Lê e descarta o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    // Lê linha por linha
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remove o \n do final, se existir
        linha[strcspn(linha, "\n")] = '\0';

        // Quebra a linha em colunas
        char *codigo    = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        char *qtde      = strtok(NULL, ";");
        char *preco     = strtok(NULL, ";");

        if (codigo && descricao && qtde && preco) {
           Produto* p = criarNovoProduto(codigo, descricao, atoi(qtde), atof(preco));
           printf("==============================\n");
           printf("Codigo: %s\n", p->cod);
           printf("Nome: %s\n", p->nome);
           printf("Quantidade: %d\n", p->quantidade);
           printf("Preco: %.2f\n", p->preco);
        }
    }

    fclose(arquivo);
    return 0;
}