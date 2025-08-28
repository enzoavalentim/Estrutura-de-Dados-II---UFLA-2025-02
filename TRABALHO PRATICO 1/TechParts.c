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
        printf("Erro de alocacao de memoria.\n");
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
    
    double A = 0.618; 
    double fracionaria = fmod(codTratado * A, 1.0);
    int indiceMulplicacao = (unsigned int)(M * fracionaria);
    
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

void excluirProduto(const char* cod) {
    if (cod == NULL) {
        printf("Codigo invalido!\n");
        return;
    }

    long int codTratado = tratamentoCodigo(cod);

    double A = 0.618; 
    double fracionaria = fmod(codTratado * A, 1.0);
    int indiceMultiplicacao = (unsigned int)(M * fracionaria);

    int indiceDivisao = codTratado % M;

    Produto *atual, *anterior;
    int removido = 0;

    atual = tabelaHash[indiceMultiplicacao];
    anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->cod, cod) == 0) {
            if (anterior == NULL)
                tabelaHash[indiceMultiplicacao] = atual->prox;
            else
                anterior->prox = atual->prox;

            free(atual);
            printf("Produto %s removido (bucket multiplicacao).\n", cod);
            removido = 1;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    atual = tabelaHash[indiceDivisao];
    anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->cod, cod) == 0) {
            if (anterior == NULL)
                tabelaHash[indiceDivisao] = atual->prox;
            else
                anterior->prox = atual->prox;

            free(atual);
            printf("Produto %s removido (bucket divisao).\n", cod);
            removido = 1;
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }

    if (!removido) {
        printf("Produto %s nao encontrado na tabela.\n", cod);
    }
}

void inserirNovoProduto(const char* cod, const char* nome, int quantidade, double preco, int indice) {
    if (indice < 0 || indice >= M) {
        fprintf(stderr, "Erro: indice de hash fora do intervalo (%d)\n", indice);
        return;
    }

    int prodJacadastrado = verificaProdutoExistente(cod);
    if (prodJacadastrado == 1) {
        printf("Codigo %s ja cadastrado. Volte ao cadastro de pecas e tente novamente.\n", cod);
        return;
    }

    Produto* p = criarNovoProduto(cod, nome, quantidade, preco);
    p->prox = tabelaHash[indice];
    tabelaHash[indice] = p;
    printf("\nProduto %s inserido com sucesso!.\n", cod);
}

void buscarProduto(const char* cod) {

    long int codTratado = tratamentoCodigo(cod);
    
    double A = 0.618; 
    double fracionaria = fmod(codTratado * A, 1.0);
    int indiceMulplicacao = (unsigned int)(M * fracionaria);

    int indiceDivisao = codTratado % M;
        

    Produto* atualMultplicacao = tabelaHash[indiceMulplicacao];
    Produto* atualDivisao = tabelaHash[indiceDivisao];

    while (atualMultplicacao != NULL) {
        if (strcmp(atualMultplicacao->cod, cod) == 0) {
            printf("\nProduto encontrado: \nCodigo: %s \nNome: %s \nQuantidade: %d \nPreco: %.2f\n", atualMultplicacao->cod, atualMultplicacao->nome, atualMultplicacao->quantidade, atualMultplicacao->preco);
            return ;
        }
        atualMultplicacao = atualMultplicacao->prox;
    }

    while (atualDivisao != NULL) {
        if (strcmp(atualDivisao->cod, cod) == 0) {
            printf("\nProduto encontrado: \nCodigo: %s \nNome: %s \nQuantidade: %d \nPreco: %.2f\n", atualDivisao->cod, atualDivisao->nome, atualDivisao->quantidade, atualDivisao->preco);
            return;  
        }
        atualDivisao = atualDivisao->prox;
    }

    printf("Produto %s nao encontrado na tabela.\n", cod);
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

void estatiticasTabela() {
    printf("\nTamanho da tabela (m): %d\n", M);

    int n = 0;
    for (int i = 0; i < M; i++) {
        Produto* atual = tabelaHash[i];
        while (atual) {
            n++;
            atual = atual->prox;
        }
    }
    printf("Numero de elementos (n): %d\n", n);

    fatorDeCarga(); 

    int bucketsUtilizados = 0;
    int bucketMaisCheio = -1;
    int maiorTamanho = 0;

    for (int i = 0; i < M; i++) {
        Produto* atual = tabelaHash[i];

        if (atual != NULL) {
            bucketsUtilizados++;
        }

        int tamanhoAtual = 0; 
        while (atual) {
            tamanhoAtual++;
            atual = atual->prox;
        }

        if (tamanhoAtual > maiorTamanho) {
            maiorTamanho = tamanhoAtual;
            bucketMaisCheio = i;
        }
    }
    printf("Numero de buckets utilizados: %d (%.2lf%%)\n", bucketsUtilizados, (bucketsUtilizados * 100) / (M * 1.0));
    printf("Maior lista (colisoes): %d\n", maiorTamanho);
}

void reHash(){

}

void importarDeCSV(char* nomeArquivo){

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\nVerifique se o nome do arquivo esta correto e tente novamente.\n");
        return;
    }

    char linha[256];

    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';

        char *codigo    = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        char *qtde      = strtok(NULL, ";");
        char *preco     = strtok(NULL, ";");

        if (codigo && descricao && qtde && preco) {
           int indice = calcHash(codigo);
           inserirNovoProduto(codigo, descricao, atoi(qtde), atof(preco), indice);
        }
    }

    fclose(arquivo);
    return 0;
}

void exportarParaCSV(const char* nomeArquivo){

    FILE* arquivo = fopen(nomeArquivo, "w");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    fprintf(arquivo, "codigo;descricao;qtde;preco\n");

    for (int i = 0; i < M; i++) {
        Produto* atual = tabelaHash[i];
        while (atual) {
           
            fprintf(arquivo, "%s;%s;%d;%.2f\n",
                    atual->cod,
                    atual->nome,
                    atual->quantidade,
                    atual->preco);
            atual = atual->prox;
        }
    }

    fclose(arquivo);
    printf("Exportacao concluida no mesmo diretorio do executavel, arquivo: %s!\n", nomeArquivo);


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

                if (leituraValida != 1) {+
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

                break;
            }
            case 2: {
                printf("\n======================\n BUSCA DE PECA\n======================\n");

                char cod[maxCod];
                printf("Digite o codigo da peca: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = '\0';
                buscarProduto(cod);
                break;
            }
            case 3: {
                printf("\n======================\n REMOCAO DE PECA\n======================\n");

                char cod[maxCod];
                printf("Digite o codigo da peca: ");
                fgets(cod, sizeof(cod), stdin);
                cod[strcspn(cod, "\n")] = '\0';
                excluirProduto(cod);
                break;
            }
            case 4: {
                printf("\n======================\n EXIBIR ESTATISTICAS\n======================\n");

                estatiticasTabela();
                break;
            }
            case 5: {
                printf("\n======================\n CARREGAR PECAS POR CSV\n======================\n");

                printf("Digite o nome do arquivo que deseja importar: ");
                printf("\n*OBS: O arquivo deve estar no mesmo diretorio do executavel e o nome padrao csv com cabeçalho {codigo;descricao;qtde;preco}\n");
                printf("\nDigite o nome do arquivo (ex: produtosTechParts.csv): ");
                char *nomeArquivo = malloc(100 * sizeof(char));
                fgets(nomeArquivo, 100, stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0'; 
                importarDeCSV(nomeArquivo);
                break;
            }
            case 6: {
                printf("\n======================\n SALVAR TABELA EM CSV\n======================\n");

                exportarParaCSV("tabelaTechParts.csv");
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
