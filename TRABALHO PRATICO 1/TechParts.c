#include <stdlib.h>


void imprimirMenu(){
    printf("===========================================\n SISTEMA DE ESTOQUE - TECHPARTS\n===========================================\n");
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
    imprimirMenu();
    return 0;
}

