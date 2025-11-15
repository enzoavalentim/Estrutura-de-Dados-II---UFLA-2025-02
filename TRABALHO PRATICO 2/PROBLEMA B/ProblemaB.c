#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RED 1
#define BLACK 0

long long total_rotacoes = 0;
long long total_comparacoes_sucessor = 0;
long long qtd_sucessor = 0;

typedef struct {
    int id;
    char titulo[61];
    long long inicio;
    long long fim;
} Evento;

typedef struct NO {
    Evento dado;
    struct NO *esq, *dir;
    int cor;
} NO;

typedef NO* ArvLLRB;

int cor(NO* h) {
    return h ? h->cor : BLACK;
}

void trocaCor(NO* h) {
    h->cor = !h->cor;
    if (h->esq) h->esq->cor = !h->esq->cor;
    if (h->dir) h->dir->cor = !h->dir->cor;
}

NO* rotacionaEsquerda(NO* h) {
    total_rotacoes++;
    NO* x = h->dir;
    h->dir = x->esq;
    x->esq = h;
    x->cor = h->cor;
    h->cor = RED;
    return x;
}

NO* rotacionaDireita(NO* h) {
    total_rotacoes++;
    NO* x = h->esq;
    h->esq = x->dir;
    x->dir = h;
    x->cor = h->cor;
    h->cor = RED;
    return x;
}

NO* balancear(NO* h) {
    if (cor(h->dir) == RED && cor(h->esq) == BLACK)
        h = rotacionaEsquerda(h);

    if (cor(h->esq) == RED && cor(h->esq->esq) == RED)
        h = rotacionaDireita(h);

    if (cor(h->esq) == RED && cor(h->dir) == RED)
        trocaCor(h);

    return h;
}

NO* insereNO(NO* h, Evento e, int* resp) {
    if (!h) {
        NO* novo = (NO*)malloc(sizeof(NO));
        novo->dado = e;
        novo->esq = novo->dir = NULL;
        novo->cor = RED;
        *resp = 1; // inserido
        return novo;
    }

    if (e.id == h->dado.id) {
        h->dado = e;
        *resp = 2; // atualizado
        return h;
    }

    if (e.id < h->dado.id)
        h->esq = insereNO(h->esq, e, resp);
    else
        h->dir = insereNO(h->dir, e, resp);

    h = balancear(h);
    return h;
}

int inserir(ArvLLRB* raiz, Evento e) {
    int r = 0;
    *raiz = insereNO(*raiz, e, &r);
    if (*raiz)
        (*raiz)->cor = BLACK;
    return r;
}


Evento* buscarID(NO* h, int id) {
    while (h) {
        if (id == h->dado.id) return &h->dado;
        if (id < h->dado.id) h = h->esq;
        else                 h = h->dir;
    }
    return NULL;
}

void listar(NO* h) {
    if (!h) return;
    listar(h->esq);
    printf("%d;%s;%lld;%lld\n", h->dado.id, h->dado.titulo, h->dado.inicio, h->dado.fim);
    listar(h->dir);
}

void listarIntervalo(NO* h, long long Ta, long long Tb) {
    if (!h) return;
    listarIntervalo(h->esq, Ta, Tb);

    if (h->dado.inicio >= Ta && h->dado.inicio <= Tb)
        printf("%d;%s;%lld;%lld\n", h->dado.id, h->dado.titulo, h->dado.inicio, h->dado.fim);

    listarIntervalo(h->dir, Ta, Tb);
}


void buscaSucessor(NO* raiz, long long T, NO** melhor) {
    if (!raiz) return;

    buscaSucessor(raiz->esq, T, melhor);

    total_comparacoes_sucessor++;

    if (raiz->dado.inicio >= T) {
        if (!*melhor || raiz->dado.inicio < (*melhor)->dado.inicio)
            *melhor = raiz;
    }

    buscaSucessor(raiz->dir, T, melhor);
}

NO* sucessorTemporal(ArvLLRB raiz, long long T) {
    qtd_sucessor++;
    NO* melhor = NULL;
    buscaSucessor(raiz, T, &melhor);
    return melhor;
}

void liberarArvore(NO* h);

NO* move2EsqRED(NO* h) {
    trocaCor(h);
    if (cor(h->dir->esq) == RED) {
        h->dir = rotacionaDireita(h->dir);
        h = rotacionaEsquerda(h);
        trocaCor(h);
    }
    return h;
}

NO* move2DirRED(NO* h) {
    trocaCor(h);
    if (cor(h->esq->esq) == RED) {
        h = rotacionaDireita(h);
        trocaCor(h);
    }
    return h;
}

NO* procuraMenor(NO* h) {
    while (h->esq) h = h->esq;
    return h;
}

NO* removerMenor(NO* h) {
    if (!h->esq) {
        free(h);
        return NULL;
    }

    if (cor(h->esq) == BLACK && cor(h->esq->esq) == BLACK)
        h = move2EsqRED(h);

    h->esq = removerMenor(h->esq);
    return balancear(h);
}

NO* remove_NO(NO* h, int id) {
    if (id < h->dado.id) {
        if (cor(h->esq) == BLACK && cor(h->esq->esq) == BLACK)
            h = move2EsqRED(h);
        h->esq = remove_NO(h->esq, id);
    } else {
        if (cor(h->esq) == RED)
            h = rotacionaDireita(h);

        if (id == h->dado.id && !h->dir) {
            free(h);
            return NULL;
        }

        if (cor(h->dir) == BLACK && cor(h->dir->esq) == BLACK)
            h = move2DirRED(h);

        if (id == h->dado.id) {
            NO* x = procuraMenor(h->dir);
            h->dado = x->dado;
            h->dir = removerMenor(h->dir);
        } else {
            h->dir = remove_NO(h->dir, id);
        }
    }
    return balancear(h);
}

int remover(ArvLLRB* raiz, int id) {
    if (!*raiz)
        return 0;

    if (cor((*raiz)->esq) == BLACK && cor((*raiz)->dir) == BLACK)
        (*raiz)->cor = RED;

    *raiz = remove_NO(*raiz, id);

    if (*raiz)
        (*raiz)->cor = BLACK;

    return 1;
}

void rstrip(char *s) {
    int len = strlen(s);
    while (len > 0 && (s[len-1]=='\n' || s[len-1]=='\r' || isspace((unsigned char)s[len-1]))) {
        s[len-1] = '\0';
        len--;
    }
}

int parseLinhaCSV(const char* linha, Evento* out) {
    char buf[1024];
    strcpy(buf, linha);
    rstrip(buf);

    char *p1 = strchr(buf, ';');
    if (!p1) return 0;
    char *p2 = strchr(p1+1, ';');
    if (!p2) return 0;
    char *p3 = strchr(p2+1, ';');
    if (!p3) return 0;

    // id
    *p1 = '\0';
    out->id = atoi(buf);

    // titulo
    *p2 = '\0';
    if (strlen(p1+1) > 60) return 0;
    strcpy(out->titulo, p1+1);

    // inicio
    *p3 = '\0';
    out->inicio = atoll(p2+1);

    // fim
    out->fim = atoll(p3+1);

    if (out->fim < out->inicio) return 0;

    return 1;
}

int parseCSVFile(const char* caminho, Evento** arrOut, size_t* nOut) {
    FILE* f = fopen(caminho, "r");
    if (!f) return 0;

    Evento* arr = NULL;
    size_t cap = 0, n = 0;
    char linha[1024];

    while (fgets(linha, sizeof(linha), f)) {
        rstrip(linha);
        if (strlen(linha)==0) continue;

        Evento e;
        if (!parseLinhaCSV(linha, &e)) {
            free(arr);
            fclose(f);
            return 0;
        }

        if (n == cap) {
            cap = cap ? cap*2 : 16;
            arr = realloc(arr, cap*sizeof(Evento));
        }
        arr[n++] = e;
    }

    fclose(f);
    *arrOut = arr;
    *nOut = n;
    return 1;
}

void salvar_inorder(NO* h, FILE* f, int* ok) {
    if (!h || !*ok) return;
    salvar_inorder(h->esq, f, ok);
    if (fprintf(f,"%d;%s;%lld;%lld\n",
                h->dado.id, h->dado.titulo,
                h->dado.inicio, h->dado.fim) < 0)
        *ok = 0;
    salvar_inorder(h->dir, f, ok);
}

int salvarCSV(ArvLLRB raiz, const char* caminho) {
    FILE* f = fopen(caminho, "w");
    if (!f) return 0;
    int ok = 1;
    salvar_inorder(raiz, f, &ok);
    fclose(f);
    return ok;
}

int carregarCSV(ArvLLRB* raiz, const char* caminho) {
    Evento* arr; size_t n;
    if (!parseCSVFile(caminho, &arr, &n)) return 0;

    liberarArvore(*raiz);
    *raiz = NULL;

    for (size_t i=0; i<n; i++)
        inserir(raiz, arr[i]);

    free(arr);
    return 1;
}

int mesclarCSV(ArvLLRB* raiz, const char* caminho) {
    Evento* arr; size_t n;
    if (!parseCSVFile(caminho, &arr, &n)) return 0;

    for (size_t i=0; i<n; i++)
        inserir(raiz, arr[i]);

    free(arr);
    return 1;
}

void liberarArvore(NO* h) {
    if (!h) return;
    liberarArvore(h->esq);
    liberarArvore(h->dir);
    free(h);
}

int contaNos(NO* h) {
    if (!h) return 0;
    return 1 + contaNos(h->esq) + contaNos(h->dir);
}

int altura(NO* h) {
    if (!h) return 0;
    int e = altura(h->esq);
    int d = altura(h->dir);
    return 1 + (e>d ? e:d);
}

void imprimirEstatisticas(ArvLLRB raiz) {
    int N = contaNos(raiz);
    int H = altura(raiz);
    double media = 0.0;

    if (qtd_sucessor > 0)
        media = (double)total_comparacoes_sucessor / qtd_sucessor;

    printf("N=%d; ALTURA=%d; ROTACOES=%lld; COMP_SUCESSOR_MEDIA=%.2f\n",
           N, H, total_rotacoes, media);
}


void processarOperacao(char* linha, ArvLLRB* raiz) {
    char* p = strchr(linha, '\n');
    if (p) *p = '\0';

    // INSERIR
    if (strncmp(linha,"INSERIR;",8)==0) {
        char* rest = linha+8;
        char* id_s = strtok(rest,";");
        char* titulo = strtok(NULL,";");
        char* inicio_s = strtok(NULL,";");
        char* fim_s = strtok(NULL,";");
        if (!id_s||!titulo||!inicio_s||!fim_s || strlen(titulo)>60) {
            printf("ERRO\n"); return;
        }
        long long ini = atoll(inicio_s);
        long long fim = atoll(fim_s);
        if (fim < ini) { printf("ERRO\n"); return; }

        Evento e = {atoi(id_s),"",ini,fim};
        strcpy(e.titulo, titulo);

        int r = inserir(raiz,e);
        if (r==1) printf("INSERIDO\n");
        else if (r==2) printf("ATUALIZADO\n");
        else printf("ERRO\n");
        return;
    }

    // BUSCAR
    if (strncmp(linha,"BUSCAR;",7)==0) {
        int id = atoi(linha+7);
        Evento* e = buscarID(*raiz,id);
        if (!e) printf("NAO_ENCONTRADO\n");
        else printf("%d;%s;%lld;%lld\n",e->id,e->titulo,e->inicio,e->fim);
        return;
    }

    // REMOVER
    if (strncmp(linha,"REMOVER;",8)==0) {
        int id = atoi(linha+8);
        Evento* e = buscarID(*raiz,id);
        if (!e) { printf("NAO_ENCONTRADO\n"); return; }
        remover(raiz,id);
        printf("REMOVIDO\n");
        return;
    }

    // LISTAR
    if (strcmp(linha,"LISTAR")==0) {
        listar(*raiz);
        return;
    }

    // INTERVALO
    if (strncmp(linha,"INTERVALO;",10)==0) {
        char* rest = linha+10;
        char* a = strtok(rest,";");
        char* b = strtok(NULL,";");
        if (!a||!b) { printf("ERRO\n"); return; }
        listarIntervalo(*raiz, atoll(a), atoll(b));
        return;
    }

    // PROXIMO
    if (strncmp(linha,"PROXIMO;",8)==0) {
        long long T = atoll(linha+8);
        NO* n = sucessorTemporal(*raiz,T);
        if (!n) printf("SEM_SUCESSOR\n");
        else printf("%d;%s;%lld;%lld\n",
                    n->dado.id,n->dado.titulo,n->dado.inicio,n->dado.fim);
        return;
    }

    // CARREGARCSV
    if (strncmp(linha,"CARREGARCSV;",12)==0) {
        char* c = linha+12;
        if (carregarCSV(raiz,c)) printf("OK\n");
        else printf("ERRO\n");
        return;
    }

    // SALVARCSV
    if (strncmp(linha,"SALVARCSV;",10)==0) {
        char* c = linha+10;
        if (salvarCSV(*raiz,c)) printf("OK\n");
        else printf("ERRO\n");
        return;
    }

    // MESCLARCSV
    if (strncmp(linha,"MESCLARCSV;",11)==0) {
        char* c = linha+11;
        if (mesclarCSV(raiz,c)) printf("OK\n");
        else printf("ERRO\n");
        return;
    }

    // ESTATISTICAS
    if (strcmp(linha,"ESTATISTICAS")==0) {
        imprimirEstatisticas(*raiz);
        return;
    }

    printf("ERRO\n");
}

int main() {
    char linha[1024];
    ArvLLRB raiz = NULL;

    fgets(linha,sizeof(linha),stdin);
    if (strcmp(linha,"PROBLEMA=B\n")!=0 &&
        strcmp(linha,"PROBLEMA=B")!=0) {
        return 0;
    }

    fgets(linha,sizeof(linha),stdin);

    while (fgets(linha,sizeof(linha),stdin)) {
        if (strcmp(linha,"FIM\n")==0 || strcmp(linha,"FIM")==0)
            break;

        Evento e;
        if (parseLinhaCSV(linha,&e)) {
            inserir(&raiz,e);
        }
    }

    fgets(linha,sizeof(linha),stdin);

    while (fgets(linha,sizeof(linha),stdin)) {
        if (strcmp(linha,"FIM\n")==0 || strcmp(linha,"FIM")==0)
            break;

        processarOperacao(linha,&raiz);
    }

    liberarArvore(raiz);
    return 0;
}
