#include <stdlib.h>


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
    printf("\n[7] Trocar funcao de hash (Divisao <-> Multiplicacao)");
    printf("\n[8] Encerrar o programa");
    printf("\n");
    printf("-------------------------------------------");
    printf("\nDigite a opcao desejada:");
}

int main() {
    int opcao;
    int resultado;

    do {
        imprimirMenu();

        resultado = scanf("%d", &opcao);

        while (getchar() != '\n');

        if (resultado != 1) {
            printf("Entrada inválida! Digite um número.\n");
            continue;
        }

        switch (opcao) {
            case 1: {
                printf("\nOpção 1");
                break;
            }
            case 2: {
                printf("\nOpção 2");
                break;
            }
            case 3: {
                printf("\nOpção 3");
                break;
            }
            case 4: {
                printf("\nOpção 4");
                break;
            }
            case 5: {
                printf("\nOpção 5");
                break;
            }
            case 6: {
                printf("\nOpção 6");
                break;
            }
            case 7: {
                printf("\nOpção 7");
                break;
            }
            case 8:
                printf("\nOpção 8");
                break;
            default:
                printf("Opção inválida! Digite entre 1 e 8.\n");
        }

    } while (opcao != 8);

    return 0;
}
