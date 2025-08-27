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

Produto* tabelaHash[M];

void inicializarTabela() {
    for (int i = 0; i < M; i++) {
        tabelaHash[i] = NULL;
    }
}

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

void inserirNovoProduto(const char* cod, const char* nome, int quantidade, double preco, int indice) {
    Produto* novoProd = tabelaHash[indice];

    while (novoProd != NULL) {
        if (novoProd->cod == cod) {
            strncpy(novoProd->nome, nome, maxNome - 1);
            novoProd->nome[maxNome - 1] = '\0';
            return;
        }
        novoProd = novoProd->prox;
    }

    Produto* p = criarNovoProduto(cod, nome, quantidade, preco);
    p->prox = tabelaHash[indice];
    tabelaHash[indice] = p;
}

void imprimirHash() {
    printf("\n--- ESTADO DA TABELA HASH ---\n");
    for (int i = 0; i < M; i++) {
        printf("[%2d]: ", i);
        Produto* atual = tabelaHash[i];
        if (!atual) {
            printf("(vazio)");
        }
        while (atual) {
            printf(" -> (Codigo:%s, Nome:%s)", atual->cod, atual->nome);
            atual = atual->prox;
        }
        printf("\n");
    }
}

void imprimirMenu(){
    printf("\n===========================================\n SISTEMA DE ESTOQUE - TECHPARTS\n===========================================\n");
    printf("\n");
    printf("Selecione uma opcao:");
    printf("\n[1] Inserir nova peca");
    printf("\n[2] Buscar peca por codigo");
    printf("\n[3] Remover peca do estoque");
    printf("\n[4] Exibir estatisticas da tabela");
    printf("\n[5] Carregar pecas de um arquivo CSV");
    printf("\n[6] Salvar tabela em arquivo CSV");
    printf("\n[7] Trocar funcao de hash (Multiplicacao <-> Divisao)");
    printf("\n[8] Encerrar o programa");
    printf("\n");
    printf("-------------------------------------------");
    printf("\nDigite a opcao desejada:");
}

char* removeCaracterEspecial(const char* cod) {
    int len = strlen(cod);
    char* tratada = malloc(len + 1); 
    if (tratada == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }

    int j = 0;
    for (int i = 0; i < len; i++) {
        if (isalnum(cod[i])) { 
            tratada[j++] = cod[i];
        }
    }
    tratada[j] = '\0';

    return tratada; 
}


long int tratamentoCodigo(char* cod) {
    char* codTratado = removeCaracterEspecial(cod);

    char saida[maxCod];         
    saida[0] = '\0';        

    for (int i = 0; codTratado[i] != '\0'; i++) {
        char c = codTratado[i];

        if (isalpha((unsigned char)c)) {
            char buffer[5];  
            snprintf(buffer, sizeof(buffer), "%d", (int)c); 
            if (strlen(saida) + strlen(buffer) < sizeof(saida))
                strcat(saida, buffer);
        } else {
            char buffer[2] = {c, '\0'};  
            if (strlen(saida) + 1 < sizeof(saida))
                strcat(saida, buffer);
        }
    }
    
    long int resultado = strtol(saida, NULL, 10);
    return resultado;
}

int calcHash(char* cod){
    long int codTratado = tratamentoCodigo(cod);
    
    if (selectFuncaoHash == 0) {
        double A = 0.618; 
        double fracionaria = fmod(codTratado * A, 1.0);
        int indice = (unsigned int)(M * fracionaria);
        return indice;

        
    } else {
        int indice = codTratado % M;
        return indice;
    }
}

void reHash(){

}

double fatorDeCarga() {
    int n = 0;
    for (int i = 0; i < M; i++) {
        Produto* atual = tabelaHash[i];
        while (atual) {
            n++;
            atual = atual->prox;
        }
    }

    double carga = (double)n / (double)M;

    if (carga > 0.75) {
        printf("Fator de carga: %.2f - Rehash necessario!\n", carga);
        reHash();
    } else {
        printf("Fator de carga: %.2f\n", carga);
    }
    return carga;


}

int main() {
    setlocale(LC_ALL, "Portuguese_Brazil");

    inicializarTabela();

    int opcao;
    int resultado;

    do {
        imprimirMenu();

        resultado = scanf("%d", &opcao);

        while (getchar() != '\n');

        if (resultado != 1) {
            printf("Entrada invalida! Digite um numero.\n");
            continue;
        }

        switch (opcao) {
            case 1: {
                printf("\n======================\n CADASTRO DE PECAS\n======================\n");
                
                if (selectFuncaoHash == 0) {
                    printf("Funcao de hash atual: Multiplicacao\n");
                } else {
                    printf("Funcao de hash atual: Divisao\n");
                }

                char nome[maxNome];
                printf("\nDigite a descricao da peca: ");
                fgets(nome, sizeof(nome), stdin);
                nome[strcspn(nome, "\n")] = '\0';

                char cod[maxCod];
                printf("Digite o codigo da peca: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = '\0';
                int indice = calcHash(cod);

                int quantEstoque;
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &quantEstoque);

                double precoUnitario;
                printf("Digite o preco unitario: ");                
                scanf("%lf", &precoUnitario);

                printf(cod);

                inserirNovoProduto(cod, nome, quantEstoque, precoUnitario, indice);
                imprimirHash();

                break;
            }
            case 2: {
                printf("\n======================\n BUSCA DE PECA\n======================\n");
                break;
            }
            case 3: {
                printf("\n======================\n REMOCAO DE PECA\n======================\n");
                break;
            }
            case 4: {
                printf("\n======================\n EXIBIR ESTATISTICAS\n======================\n");
                break;
            }
            case 5: {
                printf("\n======================\n CARREGAR PECAS POR CSV\n======================\n");
                break;
            }
            case 6: {
                printf("\n======================\n SALVAR TABELA EM CSV\n======================\n");
                break;
            }
            case 7: {
                printf("\n======================\n TROCAR FUNCAO DE HASH\n======================\n");

                printf("Selecione a funcao de hash desejada:\n");
                printf("[0] Multiplicacao\n");
                printf("[1] Divisao\n");

                int novaFuncao;
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &novaFuncao);

                selectFuncaoHash = novaFuncao;
                printf("Funcao de hash alterada com sucesso!\n");
                
                break;
            }
            case 8:
                printf("\nObrigado por usar o sistema TechParts! Ate a proxima.\n");
                break;
            default:

                printf("Opcao invalida! Digite entre 1 e 8.\n");
        }

    } while (opcao != 8);

    return 0;
}
