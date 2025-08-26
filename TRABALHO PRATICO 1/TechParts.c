#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>


#define M 13               
#define maxNome 60
#define maxCod 15
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


void tratamentoCodigo(char* cod) {
    char* codTratado = removeCaracterEspecial(cod); 
    
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
                tratamentoCodigo(cod);


                int quantEstoque;
                printf("Digite a quantidade em estoque: ");
                scanf("%d", &quantEstoque);

                double precoUnitario;
                printf("Digite o preco unitario: ");                
                scanf("%lf", &precoUnitario);



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
