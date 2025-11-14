#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *s) {
    char *p = malloc(strlen(s) + 1);
    if (!p) return NULL;
    strcpy(p, s);
    return p;
}

typedef struct NO {
    char *codigo;
    char *descricao;
    int estoque;
    float preco;
    int altura;
    struct NO *esq;
    struct NO *dir;
} NO;

typedef struct AVLTree {
    NO *raiz;
    int N;
    int rotacoes;
    long long comp_busca;
    int num_buscas;
} AVLTree;

AVLTree* criarAVL() {
    AVLTree *arv = (AVLTree*) malloc(sizeof(AVLTree));
    if (!arv) return NULL;

    arv->raiz = NULL;
    arv->N = 0;
    arv->rotacoes = 0;
    arv->comp_busca = 0;
    arv->num_buscas = 0;

    return arv;
}

NO* criarNo(char *codigo, char *descricao, int estoque, float preco) {
    NO *no = (NO*) malloc(sizeof(NO));
    if (!no) return NULL;

    no->codigo = my_strdup(codigo);
    no->descricao = my_strdup(descricao);
    no->estoque = estoque;
    no->preco = preco;
    no->altura = 0;
    no->esq = NULL;
    no->dir = NULL;

    return no;
}

int alturaNo(NO *no) {
    if (no == NULL) return -1;
    return no->altura;
}

int fatorBalanceamento(NO *no) {
    return alturaNo(no->esq) - alturaNo(no->dir);
}

void atualizarAltura(NO *no) {
    int h_esq = alturaNo(no->esq);
    int h_dir = alturaNo(no->dir);
    no->altura = (h_esq > h_dir ? h_esq : h_dir) + 1;
}

NO* rotacaoLL(NO *no, AVLTree *arv) {
    NO *p = no->esq;
    no->esq = p->dir;
    p->dir = no;

    atualizarAltura(no);
    atualizarAltura(p);

    arv->rotacoes++;
    return p;
}

NO* rotacaoRR(NO *no, AVLTree *arv) {
    NO *p = no->dir;
    no->dir = p->esq;
    p->esq = no;

    atualizarAltura(no);
    atualizarAltura(p);

    arv->rotacoes++;
    return p;
}

NO* rotacaoLR(NO *no, AVLTree *arv) {
    no->esq = rotacaoRR(no->esq, arv);
    return rotacaoLL(no, arv);
}

NO* rotacaoRL(NO *no, AVLTree *arv) {
    no->dir = rotacaoLL(no->dir, arv);
    return rotacaoRR(no, arv);
}

NO* inserirRec(NO *no, AVLTree *arv,
               char *codigo,
               char *descricao,
               int estoque,
               float preco,
               int *resultado)
{
    if (no == NULL) {
        NO *novo = criarNo(codigo, descricao, estoque, preco);
        if (!novo) {
            *resultado = 0;
            return NULL;
        }
        *resultado = 1; 
        arv->N++;
        return novo;
    }

    int cmp = strcmp(codigo, no->codigo);

    if (cmp == 0) {
        free(no->descricao);
        no->descricao = my_strdup(descricao);
        no->estoque = estoque;
        no->preco = preco;

        *resultado = 2;
        return no;
    }

    else if (cmp < 0) {
        no->esq = inserirRec(no->esq, arv,
                             codigo, descricao,
                             estoque, preco,
                             resultado);
    }

    else {
        no->dir = inserirRec(no->dir, arv,
                             codigo, descricao,
                             estoque, preco,
                             resultado);
    }

    atualizarAltura(no);

    int fb = fatorBalanceamento(no);

    // Caso LL
    if (fb >= 2 && strcmp(codigo, no->esq->codigo) < 0) {
        return rotacaoLL(no, arv);
    }

    // Caso LR
    if (fb >= 2 && strcmp(codigo, no->esq->codigo) > 0) {
        return rotacaoLR(no, arv);
    }

    // Caso RR
    if (fb <= -2 && strcmp(codigo, no->dir->codigo) > 0) {
        return rotacaoRR(no, arv);
    }

    // Caso RL
    if (fb <= -2 && strcmp(codigo, no->dir->codigo) < 0) {
        return rotacaoRL(no, arv);
    }

    return no;
}

int inserirAVL(AVLTree *arv,
               char *codigo,
               char *descricao,
               int estoque,
               float preco)
{
    int res = 0;

    arv->raiz = inserirRec(arv->raiz, arv,
                           codigo, descricao,
                           estoque, preco,
                           &res);

    return res;
}

NO* buscarRec(NO *no, const char *codigo, AVLTree *arv) {
    if (no == NULL)
        return NULL;

    arv->comp_busca++;
    int cmp = strcmp(codigo, no->codigo);

    if (cmp == 0)
        return no;

    if (cmp < 0)
        return buscarRec(no->esq, codigo, arv);
    else
        return buscarRec(no->dir, codigo, arv);
}

NO* buscarAVL(AVLTree *arv, const char *codigo) {
    arv->num_buscas++;
    return buscarRec(arv->raiz, codigo, arv);
}

void listarRec(NO *no) {
    if (no == NULL)
        return;

    listarRec(no->esq);

    printf("%s;%s;%d;%.2f\n",
           no->codigo,
           no->descricao,
           no->estoque,
           no->preco);

    listarRec(no->dir);
}

void listarAVL(AVLTree *arv) {
    listarRec(arv->raiz);
}

void intervaloRec(NO *no, const char *L, const char *R) {
    if (no == NULL)
        return;

    if (strcmp(no->codigo, L) >= 0)
        intervaloRec(no->esq, L, R);

    if (strcmp(no->codigo, L) >= 0 && strcmp(no->codigo, R) <= 0) {
        printf("%s;%s;%d;%.2f\n",
               no->codigo,
               no->descricao,
               no->estoque,
               no->preco);
    }

    if (strcmp(no->codigo, R) <= 0)
        intervaloRec(no->dir, L, R);
}

void intervaloAVL(AVLTree *arv, const char *L, const char *R) {
    intervaloRec(arv->raiz, L, R);
}

NO* menorNo(NO *no) {
    while (no->esq != NULL)
        no = no->esq;
    return no;
}

void liberarNo(NO *no) {
    if (!no) return;
    free(no->codigo);
    free(no->descricao);
    free(no);
}

NO* removerRec(NO *no, AVLTree *arv,
               const char *codigo,
               int *resultado)
{
    if (no == NULL) {
        *resultado = 0; 
        return NULL;
    }

    int cmp = strcmp(codigo, no->codigo);

    if (cmp < 0) {
        no->esq = removerRec(no->esq, arv, codigo, resultado);
    }
    else if (cmp > 0) {
        no->dir = removerRec(no->dir, arv, codigo, resultado);
    }
    else {
        *resultado = 1;
        arv->N--;

        // Caso 1: nó folha
        if (no->esq == NULL && no->dir == NULL) {
            liberarNo(no);
            return NULL;
        }

        // Caso 2: um filho à direita
        if (no->esq == NULL) {
            NO *temp = no->dir;
            liberarNo(no);
            return temp;
        }

        // Caso 3: um filho à esquerda
        if (no->dir == NULL) {
            NO *temp = no->esq;
            liberarNo(no);
            return temp;
        }

        // Caso 4: dois filhos — usar sucessor (menor da direita)
        NO *sucessor = menorNo(no->dir);

        free(no->codigo);
        free(no->descricao);

        no->codigo = my_strdup(sucessor->codigo);
        no->descricao = my_strdup(sucessor->descricao);
        no->estoque = sucessor->estoque;
        no->preco = sucessor->preco;

        no->dir = removerRec(no->dir, arv, sucessor->codigo, resultado);
    }

    atualizarAltura(no);

    int fb = fatorBalanceamento(no);

    // LL
    if (fb >= 2 && fatorBalanceamento(no->esq) >= 0)
        return rotacaoLL(no, arv);

    // LR
    if (fb >= 2 && fatorBalanceamento(no->esq) < 0)
        return rotacaoLR(no, arv);

    // RR
    if (fb <= -2 && fatorBalanceamento(no->dir) <= 0)
        return rotacaoRR(no, arv);

    // RL
    if (fb <= -2 && fatorBalanceamento(no->dir) > 0)
        return rotacaoRL(no, arv);

    return no;
}

int removerAVL(AVLTree *arv, const char *codigo) {
    int res = 0;
    arv->raiz = removerRec(arv->raiz, arv, codigo, &res);
    return res;  
}

int alturaArvore(AVLTree *arv) {
    if (arv == NULL || arv->raiz == NULL) return 0;
    return arv->raiz->altura + 1;
}

void imprimirEstatisticasA(AVLTree *arv) {
    int n = (arv ? arv->N : 0);
    int altura = alturaArvore(arv);
    int rot = (arv ? arv->rotacoes : 0);

    double media = 0.0;
    if (arv && arv->num_buscas > 0) {
        media = (double) arv->comp_busca / (double) arv->num_buscas;
    }

    printf("N=%d; ALTURA=%d; ROTACOES=%d; COMP_BUSCA_MEDIA=%.2f\n",
           n, altura, rot, media);
}

void liberarArvoreRec(NO *no) {
    if (no == NULL)
        return;

    liberarArvoreRec(no->esq);
    liberarArvoreRec(no->dir);

    free(no->codigo);
    free(no->descricao);
    free(no);
}

void liberarAVL(AVLTree *arv) {
    if (!arv) return;

    liberarArvoreRec(arv->raiz);

    free(arv);
}

char* lerLinha(char *buffer, int tam) {
    if (fgets(buffer, tam, stdin) == NULL)
        return NULL;

    buffer[strcspn(buffer, "\n")] = '\0';  
    return buffer;
}


void lerRegistrosIniciais(AVLTree *arv) {
    char linha[256];

    while (lerLinha(linha, sizeof(linha))) {
        if (strcmp(linha, "FIM") == 0)
            break;

        char *codigo = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        char *estoqueStr = strtok(NULL, ";");
        char *precoStr = strtok(NULL, ";");

        if (!codigo || !descricao || !estoqueStr || !precoStr) {
            printf("ERRO\n");
            continue;
        }

        char *endptr;
        long estoque = strtol(estoqueStr, &endptr, 10);
        if (*endptr != '\0' || estoque < 0) {
            printf("ERRO\n");
            continue;
        }

        float preco = strtof(precoStr, &endptr);
        if (*endptr != '\0' || preco <= 0) {
            printf("ERRO\n");
            continue;
        }
        inserirAVL(arv, codigo, descricao, estoque, preco);
    }
}

void lerOperacoes(AVLTree *arv) {
    char linha[512];

    while (lerLinha(linha, sizeof(linha))) {

        if (strcmp(linha, "FIM") == 0)
            break;

        if (strncmp(linha, "INSERIR;", 8) == 0) {
            char *rest = linha + 8;

            char *codigo = strtok(rest, ";");
            char *descricao = strtok(NULL, ";");
            char *estoqueStr = strtok(NULL, ";");
            char *precoStr = strtok(NULL, ";");

            if (!codigo || !descricao || !estoqueStr || !precoStr) {
                printf("ERRO\n");
                continue;
            }

            char *end;
            long estoque = strtol(estoqueStr, &end, 10);
            if (*end != '\0' || estoque < 0) {
                printf("ERRO\n");
                continue;
            }

            float preco = strtof(precoStr, &end);
            if (*end != '\0' || preco <= 0) {
                printf("ERRO\n");
                continue;
            }

            int r = inserirAVL(arv, codigo, descricao, estoque, preco);
            if (r == 1) printf("INSERIDO\n");
            else if (r == 2) printf("ATUALIZADO\n");
            else printf("ERRO\n");

            continue;
        }

        if (strncmp(linha, "BUSCAR;", 7) == 0) {
            char *codigo = linha + 7;

            NO *no = buscarAVL(arv, codigo);

            if (!no) {
                printf("NAO_ENCONTRADO\n");
            } else {
                printf("%s;%s;%d;%.2f\n",
                       no->codigo, no->descricao,
                       no->estoque, no->preco);
            }

            continue;
        }

        if (strncmp(linha, "REMOVER;", 8) == 0) {
            char *codigo = linha + 8;

            int r = removerAVL(arv, codigo);
            if (r == 1) printf("REMOVIDO\n");
            else       printf("NAO_ENCONTRADO\n");

            continue;
        }

        if (strcmp(linha, "LISTAR") == 0) {
            listarAVL(arv);
            continue;
        }

        if (strncmp(linha, "INTERVALO;", 10) == 0) {
            char *rest = linha + 10;

            char *L = strtok(rest, ";");
            char *R = strtok(NULL, ";");

            if (!L || !R) {
                printf("ERRO\n");
                continue;
            }

            intervaloAVL(arv, L, R);
            continue;
        }

        if (strcmp(linha, "ESTATISTICAS") == 0) {
            imprimirEstatisticasA(arv);
            continue;
        }

        printf("ERRO\n");
    }
}

void salvarCSVRec(NO *no, FILE *f) {
    if (!no) return;

    salvarCSVRec(no->esq, f);

    fprintf(f, "%s;%s;%d;%.2f\n",
            no->codigo,
            no->descricao,
            no->estoque,
            no->preco);

    salvarCSVRec(no->dir, f);
}

void salvarCSV(AVLTree *arv, const char *caminho) {
    FILE *f = fopen(caminho, "w");
    if (!f) {
        printf("ERRO\n");
        return;
    }

    salvarCSVRec(arv->raiz, f);
    fclose(f);

    printf("OK\n");
}

void carregarCSV(AVLTree *arv, const char *caminho) {
    FILE *f = fopen(caminho, "r");
    if (!f) {
        printf("ERRO\n");
        return;
    }

    char linha[512];
    int erro = 0;

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';

        char *codigo = strtok(linha, ";");
        char *descricao = strtok(NULL, ";");
        char *estoqueStr = strtok(NULL, ";");
        char *precoStr = strtok(NULL, ";");

        if (!codigo || !descricao || !estoqueStr || !precoStr) {
            erro = 1;
            break;
        }

        char *end;
        long estoque = strtol(estoqueStr, &end, 10);
        if (*end != '\0' || estoque < 0) {
            erro = 1;
            break;
        }

        float preco = strtof(precoStr, &end);
        if (*end != '\0' || preco <= 0) {
            erro = 1;
            break;
        }

        inserirAVL(arv, codigo, descricao, estoque, preco);
    }

    fclose(f);

    if (erro)
        printf("ERRO\n");
    else
        printf("OK\n");
}



int main() {
    char linha[256];

    if (!lerLinha(linha, sizeof(linha)))
        return 0;

    if (strcmp(linha, "PROBLEMA=A") != 0) {
        return 0;
    }

    AVLTree *arv = criarAVL();

    lerLinha(linha, sizeof(linha));

    lerRegistrosIniciais(arv);

    lerLinha(linha, sizeof(linha));

    lerOperacoes(arv);

    liberarAVL(arv);
    return 0;
}
