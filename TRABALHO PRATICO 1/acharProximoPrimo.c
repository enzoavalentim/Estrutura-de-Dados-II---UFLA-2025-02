#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>

int M = 101;

bool ehPrimo(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Função que dobra o número e encontra o próximo primo
int dobrarEProximoPrimo(int n) {
    int valor = n * 2;   // dobra o número
    valor++;             // começa a procurar do próximo número
    
    while (!ehPrimo(valor)) {
        valor++;
    }
    
    return valor;
}

int main() {

    int M = 101;
    int novoM = dobrarEProximoPrimo(M);
    printf("Novo valor de M (próximo primo após dobrar %d): %d\n", M, novoM);
    

}