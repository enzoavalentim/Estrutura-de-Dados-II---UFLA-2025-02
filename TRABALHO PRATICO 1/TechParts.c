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

int verificaProdutoExistente(const char* cod) {

    long int codTratado = tratamentoCodigo(cod);
    
    //Verificando indice pela função de multiplicação
    double A = 0.618; 
    double fracionaria = fmod(codTratado * A, 1.0);
    int indiceMulplicacao = (unsigned int)(M * fracionaria);
    
    //Verificando indice pela função de divisão
    int indiceDivisao = codTratado % M;
        

    Produto* atualMultplicacao = tabelaHash[indiceMulplicacao];
    Produto* atualDivisao = tabelaHash[indiceDivisao];

    while (atualMultplicacao != NULL) {
        if (strcmp(atualMultplicacao->cod, cod) == 0) {
            return 1;  
        }
        atualMultplicacao = atualMultplicacao->prox;
    }

    while (atualDivisao != NULL) {
        if (strcmp(atualDivisao->cod, cod) == 0) {
            return 1;  
        }
        atualDivisao = atualDivisao->prox;
    }

    return 0;  
}

void inserirNovoProduto(const char* cod, const char* nome, int quantidade, double preco, int indice) {
    if (indice < 0 || indice >= M) {
        fprintf(stderr, "Erro: indice de hash fora do intervalo (%d)\n", indice);
        return;
    }

    int prodJacadastrado = verificaProdutoExistente(cod);
    if (prodJacadastrado == 1) {
        printf("Código %s já cadastrado. Volte ao cadastro de peças e tente novamente.\n", cod);
        return;
    }

    Produto* p = criarNovoProduto(cod, nome, quantidade, preco);
    p->prox = tabelaHash[indice];
    tabelaHash[indice] = p;
}

void buscarProduto(const char* cod) {
    int indice = calcHash(cod);
    Produto* atual = tabelaHash[indice];

    while (atual != NULL) {
        if (strcmp(atual->cod, cod) == 0) {
            printf("\nProduto encontrado: \nCodigo: %s \nNome: %s \nQuantidade: %d \nPreco: %.2f\n", atual->cod, atual->nome, atual->quantidade, atual->preco);
            return;
        }
        atual = atual->prox;
    }
    printf("\nProduto com codigo %s nao encontrado.\n", cod);
    return;
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
                double precoUnitario;
                int leituraValida;

                while (1) {
                printf("Digite a quantidade em estoque: ");
                leituraValida = scanf("%d", &quantEstoque);

                if (leituraValida != 1) {
                    printf("Entrada invalida! Digite um numero inteiro.\n");
                    while (getchar() != '\n'); 
                    continue;
                }

                if (quantEstoque < 0) {
                    printf("A quantidade nao pode ser negativa.\n");
                    continue;
                }

                break;
                }

                while (1) {
                printf("Digite o preco unitario: ");
                leituraValida = scanf("%lf", &precoUnitario);

                if (leituraValida != 1) {
                    printf("Entrada invalida! Digite um numero decimal.\n");
                    while (getchar() != '\n'); 
                    continue;
                }

                if (precoUnitario < 0) {
                    printf("O preco nao pode ser negativo.\n");
                    continue;
                }

                break;
                }

                inserirNovoProduto(cod, nome, quantEstoque, precoUnitario, indice);
                imprimirHash();

                break;
            }
            case 2: {
                printf("\n======================\n BUSCA DE PECA\n======================\n");

                char cod[maxCod];
                printf("Digite o codigo da peca: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = '\0';
                int indice = calcHash(cod);
                buscarProduto(cod);
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

                int novaFuncao;
                int leituraValida;

                printf("Selecione a funcao de hash desejada:\n");
                printf("[0] Multiplicacao\n");
                printf("[1] Divisao\n");

                while (1) {
                    printf("Digite a opcao desejada: ");
                    leituraValida = scanf("%d", &novaFuncao);

                    if (leituraValida != 1) {
                        printf("Entrada invalida! Digite um numero inteiro.\n");
                        while (getchar() != '\n');
                        continue;
                    }

                    if (novaFuncao < 0 || novaFuncao > 1) {
                        printf("Opcao invalida! Digite 0 ou 1.\n");
                        continue;
                    }

                    break;
                }

                selectFuncaoHash = novaFuncao;
                printf("Funcao de hash alterada com sucesso! (%d)\n", selectFuncaoHash);
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
