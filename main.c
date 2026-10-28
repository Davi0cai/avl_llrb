#include <stdio.h>
#include <stdlib.h>
//Usado para os testes dos macros
#include <time.h>

/*
 * Implementacao de:
 * 1. Arvore AVL
 * 2. Arvore LLRB (Left-Leaning Red-Black Tree)
 *
 * Valores inteiros, sem duplicatas.
 */

#define AVL 1
#define LLRB 2

#define RED 1
#define BLACK 0

#define SIZEMACRO 1000000 //define o tamanho do for usado dentro das opções de macro - troque aqui para verificar diferentes tempos, evite valores ímpares

typedef struct No {
    int chave;
    int altura;       /* Utilizado pela AVL */
    int cor;          /* Utilizado pela LLRB */
    struct No *esq;
    struct No *dir;
} No;

/* =========================================================
   FUNCOES GERAIS
   ========================================================= */

No *criarNo(int chave) {
    No *novo = malloc(sizeof(No));

    if (novo == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente.\n");
        exit(EXIT_FAILURE);
    }

    novo->chave = chave;
    novo->altura = 1;
    novo->cor = RED;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

int maximo(int a, int b) {
    return a > b ? a : b;
}

int altura(No *no) {
    return no == NULL ? 0 : no->altura;
}

No *buscar(No *raiz, int chave) {
    while (raiz != NULL) {
        if (chave == raiz->chave)
            return raiz;
        else if (chave < raiz->chave)
            raiz = raiz->esq;
        else
            raiz = raiz->dir;
    }

    return NULL;
}

void emOrdem(No *raiz) {
    if (raiz == NULL)
        return;

    emOrdem(raiz->esq);
    printf("%d ", raiz->chave);
    emOrdem(raiz->dir);
}

void liberarArvore(No *raiz) {
    if (raiz == NULL)
        return;

    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

/* =========================================================
   ARVORE AVL
   ========================================================= */

int fatorBalanceamento(No *no) {
    if (no == NULL)
        return 0;

    return altura(no->esq) - altura(no->dir);
}

void atualizarAltura(No *no) {
    no->altura = 1 + maximo(altura(no->esq),
                           altura(no->dir));
}

No *rotacaoDireitaAVL(No *y) {
    No *x = y->esq;
    No *subarvore = x->dir;

    x->dir = y;
    y->esq = subarvore;

    atualizarAltura(y);
    atualizarAltura(x);

    return x;
}

No *rotacaoEsquerdaAVL(No *x) {
    No *y = x->dir;
    No *subarvore = y->esq;

    y->esq = x;
    x->dir = subarvore;

    atualizarAltura(x);
    atualizarAltura(y);

    return y;
}

No *balancearAVL(No *no) {
    if (no == NULL)
        return NULL;

    atualizarAltura(no);

    int fb = fatorBalanceamento(no);

    /* Caso LL ou LR */
    if (fb > 1) {
        if (fatorBalanceamento(no->esq) < 0)
            no->esq = rotacaoEsquerdaAVL(no->esq);

        return rotacaoDireitaAVL(no);
    }

    /* Caso RR ou RL */
    if (fb < -1) {
        if (fatorBalanceamento(no->dir) > 0)
            no->dir = rotacaoDireitaAVL(no->dir);

        return rotacaoEsquerdaAVL(no);
    }

    return no;
}

No *inserirAVL(No *raiz, int chave) {
    if (raiz == NULL)
        return criarNo(chave);

    if (chave < raiz->chave)
        raiz->esq = inserirAVL(raiz->esq, chave);
    else if (chave > raiz->chave)
        raiz->dir = inserirAVL(raiz->dir, chave);
    else
        return raiz; /* Nao permite duplicatas */

    return balancearAVL(raiz);
}

No *menorNo(No *raiz) {
    while (raiz != NULL && raiz->esq != NULL)
        raiz = raiz->esq;

    return raiz;
}

No *removerAVL(No *raiz, int chave) {
    if (raiz == NULL)
        return NULL;

    if (chave < raiz->chave) {
        raiz->esq = removerAVL(raiz->esq, chave);
    } else if (chave > raiz->chave) {
        raiz->dir = removerAVL(raiz->dir, chave);
    } else {
        /* No com zero ou um filho */
        if (raiz->esq == NULL || raiz->dir == NULL) {
            No *filho = raiz->esq != NULL
                      ? raiz->esq : raiz->dir;

            free(raiz);
            return filho;
        }

        /* No com dois filhos: sucessor em ordem */
        No *sucessor = menorNo(raiz->dir);
        raiz->chave = sucessor->chave;
        raiz->dir = removerAVL(raiz->dir,
                               sucessor->chave);
    }

    return balancearAVL(raiz);
}

/* =========================================================
   ARVORE LLRB
   ========================================================= */

int ehVermelho(No *no) {
    return no != NULL && no->cor == RED;
}

void trocarCores(No *no) {
    if (no == NULL)
        return;

    no->cor = !no->cor;

    if (no->esq != NULL)
        no->esq->cor = !no->esq->cor;

    if (no->dir != NULL)
        no->dir->cor = !no->dir->cor;
}

No *rotacaoEsquerdaLLRB(No *h) {
    No *x = h->dir;

    h->dir = x->esq;
    x->esq = h;

    x->cor = h->cor;
    h->cor = RED;

    return x;
}

No *rotacaoDireitaLLRB(No *h) {
    No *x = h->esq;

    h->esq = x->dir;
    x->dir = h;

    x->cor = h->cor;
    h->cor = RED;

    return x;
}

No *corrigirLLRB(No *h) {
    if (h == NULL)
        return NULL;

    /* Ligacao vermelha inclinada para a direita */
    if (ehVermelho(h->dir) && !ehVermelho(h->esq))
        h = rotacaoEsquerdaLLRB(h);

    /* Duas ligacoes vermelhas consecutivas a esquerda */
    if (ehVermelho(h->esq) && ehVermelho(h->esq->esq))
        h = rotacaoDireitaLLRB(h);

    /* Dois filhos vermelhos: inverter as cores */
    if (ehVermelho(h->esq) && ehVermelho(h->dir))
        trocarCores(h);

    return h;
}

No *inserirLLRB(No *h, int chave) {
    if (h == NULL)
        return criarNo(chave);

    if (chave < h->chave)
        h->esq = inserirLLRB(h->esq, chave);
    else if (chave > h->chave)
        h->dir = inserirLLRB(h->dir, chave);
    else
        return h;

    return corrigirLLRB(h);
}

No *moverVermelhoEsquerda(No *h) {
    trocarCores(h);

    if (h->dir != NULL && ehVermelho(h->dir->esq)) {
        h->dir = rotacaoDireitaLLRB(h->dir);
        h = rotacaoEsquerdaLLRB(h);
        trocarCores(h);
    }

    return h;
}

No *moverVermelhoDireita(No *h) {
    trocarCores(h);

    if (h->esq != NULL && ehVermelho(h->esq->esq)) {
        h = rotacaoDireitaLLRB(h);
        trocarCores(h);
    }

    return h;
}

No *minimoLLRB(No *h) {
    if (h == NULL)
        return NULL;

    while (h->esq != NULL)
        h = h->esq;

    return h;
}

No *removerMinimoLLRB(No *h) {
    if (h->esq == NULL) {
        free(h);
        return NULL;
    }

    if (!ehVermelho(h->esq) &&
        !ehVermelho(h->esq->esq)) {
        h = moverVermelhoEsquerda(h);
    }

    h->esq = removerMinimoLLRB(h->esq);

    return corrigirLLRB(h);
}

No *removerLLRB(No *h, int chave) {
    if (h == NULL)
        return NULL;

    if (chave < h->chave) {
        if (h->esq != NULL) {
            if (!ehVermelho(h->esq) &&
                !ehVermelho(h->esq->esq)) {
                h = moverVermelhoEsquerda(h);
            }

            h->esq = removerLLRB(h->esq, chave);
        }
    } else {
        if (ehVermelho(h->esq))
            h = rotacaoDireitaLLRB(h);

        if (chave == h->chave && h->dir == NULL) {
            free(h);
            return NULL;
        }

        if (h->dir != NULL) {
            if (!ehVermelho(h->dir) &&
                !ehVermelho(h->dir->esq)) {
                h = moverVermelhoDireita(h);
            }

            if (chave == h->chave) {
                No *sucessor = minimoLLRB(h->dir);

                h->chave = sucessor->chave;
                h->dir = removerMinimoLLRB(h->dir);
            } else {
                h->dir = removerLLRB(h->dir, chave);
            }
        }
    }

    return corrigirLLRB(h);
}

/* =========================================================
   IMPRESSAO VISUAL
   ========================================================= */

/*
 * A arvore e impressa de lado:
 * primeiro a subarvore direita,
 * depois o no atual,
 * depois a subarvore esquerda.
 *
 * Os nos mais a direita aparecem no topo.
 */

void imprimirArvore(No *raiz, int tipo, int nivel) {
    if (raiz == NULL)
        return;

    imprimirArvore(raiz->dir, tipo, nivel + 1);

    for (int i = 0; i < nivel; i++)
        printf("    ");

    if (tipo == AVL) {
        printf("%d [h=%d, fb=%d]\n",
               raiz->chave,
               raiz->altura,
               fatorBalanceamento(raiz));
    } else {
        printf("%d [%s]\n",
               raiz->chave,
               raiz->cor == RED ? "V" : "P");
    }

    imprimirArvore(raiz->esq, tipo, nivel + 1);
}

/* =========================================================
   MENU
   ========================================================= */

int lerInteiro(const char *mensagem, int *valor) {
    printf("%s", mensagem);

    if (scanf("%d", valor) != 1) {
        int c;

        while ((c = getchar()) != '\n' && c != EOF)
            ;

        printf("Entrada invalida. Digite um numero inteiro.\n");
        return 0;
    }

    return 1;
}

//Preparando macros
void macroCrescente(No *raiz, int tipo);

void macroDecrescente(No *raiz, int tipo);

void macroSemOrdem(No *raiz, int tipo);


void executarMenu(int tipo) {
    No *raiz = NULL;
    int opcao = -1, valor;

    do {
        printf("\n=================================\n");
        printf("Arvore %s\n", tipo == AVL ? "AVL" : "LLRB");
        printf("=================================\n");
        printf("1. Adicionar elemento\n");
        printf("2. Remover elemento\n");
        printf("3. Buscar elemento\n");
        printf("4. Exibir em ordem crescente\n");
        printf("5. Imprimir arvore\n");
        printf("6. Macro Crescente\n");
        printf("7. Macro Decrescente\n");
        printf("8. Macro Sem Ordem\n");
        printf("0. Voltar\n");
        printf("Escolha: ");

        if (!lerInteiro("", &opcao))
            continue;

        switch (opcao) {
            case 1:
                if (!lerInteiro("Valor a adicionar: ", &valor))
                    break;

                if (buscar(raiz, valor) != NULL) {
                    printf("O elemento %d ja existe.\n", valor);
                    break;
                }

                if (tipo == AVL) {
                    raiz = inserirAVL(raiz, valor);
                } else {
                    raiz = inserirLLRB(raiz, valor);
                    raiz->cor = BLACK;
                }

                printf("Elemento %d adicionado.\n", valor);
                break;

            case 2:
                if (!lerInteiro("Valor a remover: ", &valor))
                    break;

                if (buscar(raiz, valor) == NULL) {
                    printf("Elemento %d nao encontrado.\n", valor);
                    break;
                }

                if (tipo == AVL) {
                    raiz = removerAVL(raiz, valor);
                } else {
                    /*
                     * A raiz pode ficar temporariamente vermelha
                     * para permitir a remocao.
                     */
                    if (raiz != NULL &&
                        !ehVermelho(raiz->esq) &&
                        !ehVermelho(raiz->dir)) {
                        raiz->cor = RED;
                    }

                    raiz = removerLLRB(raiz, valor);

                    if (raiz != NULL)
                        raiz->cor = BLACK;
                }

                printf("Elemento %d removido.\n", valor);
                break;

            case 3:
                if (!lerInteiro("Valor a buscar: ", &valor))
                    break;

                if (buscar(raiz, valor) != NULL)
                    printf("Elemento %d encontrado.\n", valor);
                else
                    printf("Elemento %d nao encontrado.\n", valor);
                break;

            case 4:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                } else {
                    printf("Ordem crescente: ");
                    emOrdem(raiz);
                    printf("\n");
                }
                break;

            case 5:
                if (raiz == NULL) {
                    printf("A arvore esta vazia.\n");
                } else {
                    printf("\n");
                    imprimirArvore(raiz, tipo, 0);
                    printf("\n");

                    if (tipo == AVL)
                        printf("h = altura; fb = fator de balanceamento\n");
                    else
                        printf("V = vermelho; P = preto\n");
                }
                break;
                
            case 6:
                if(tipo == AVL){
                    macroCrescente(raiz, AVL);
                }else macroCrescente(raiz, LLRB);
                break;
                
            case 7:
                if(tipo == AVL){
                    macroDecrescente(raiz, AVL);
                }else macroDecrescente(raiz, LLRB);
                break;
                
            case 8:
                if(tipo == AVL){
                    macroSemOrdem(raiz, AVL);
                }else macroSemOrdem(raiz, LLRB);
                break;

            case 0:
                printf("Voltando ao menu principal...\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    liberarArvore(raiz);
}


//CÓDIGO MAIN
int main(void) {
    int opcao = -1;

    do {
        printf("\n=================================\n");
        printf("       ARVORES BALANCEADAS\n");
        printf("=================================\n");
        printf("1. Arvore AVL\n");
        printf("2. Arvore LLRB\n");
        printf("0. Encerrar programa\n");

        if (!lerInteiro("Escolha o tipo de arvore: ", &opcao))
            continue;

        switch (opcao) {
            case 1:
                executarMenu(AVL);
                break;

            case 2:
                executarMenu(LLRB);
                break;

            case 0:
                printf("Programa encerrado.\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}




//Seção do código para os macros de teste - Autoria MINHA, não da IA.
//Junto dessa parte do código sendo feita por mim, as opções do switch 6-8 também são

//Esse macro executa a adição, busca e remoção dos elementos na árvore em elementos crescentes
void macroCrescente(No* raiz, int tipo){
    clock_t inicio, fim;
    double tempoInserir, tempoRemover, tempoBusca;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO; i++){
        if(tipo == AVL){
            raiz = inserirAVL(raiz, i);
        } else raiz = inserirLLRB(raiz, i);
    }
    
    fim = clock();
    //imprimirArvore(raiz, tipo, 0); //Remova o comentário dessa linha para visualizar árvore gerada pelo macro
    tempoInserir = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO; i++){
        buscar(raiz, i);
    }
    
    fim = clock();
    tempoBusca = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO; i++){
        if(tipo == AVL){
            raiz = removerAVL(raiz, i);
        } else raiz = removerLLRB(raiz, i);
    }
    
    fim = clock();
    tempoRemover = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Tempo insercao: %fs; Tempo busca: %fs; Tempo remocao: %fs (Ordem crescente)\n", tempoInserir, tempoBusca, tempoRemover);
}

//Esse macro executa a adição, busca e remoção dos elementos na árvore em elementos decrescentes
void macroDecrescente(No* raiz, int tipo){
    clock_t inicio, fim;
    double tempoInserir, tempoRemover, tempoBusca;
    
    inicio = clock();
    
    for(int i = SIZEMACRO; i > 0; i--){
        if(tipo == AVL){
            raiz = inserirAVL(raiz, i);
        } else raiz = inserirLLRB(raiz, i);
    }
    
    fim = clock();
    //imprimirArvore(raiz, tipo, 0); //Remova o comentário dessa linha para visualizar árvore gerada pelo macro
    tempoInserir = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = SIZEMACRO; i > 0; i--){
        buscar(raiz, i);
    }
    
    fim = clock();
    tempoBusca = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = SIZEMACRO; i > 0; i--){
        if(tipo == AVL){
            raiz = removerAVL(raiz, i);
        } else raiz = removerLLRB(raiz, i);
    }
    
    fim = clock();
    tempoRemover = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Tempo insercao: %fs; Tempo busca: %fs; Tempo remocao: %fs (Ordem decrescente)\n", tempoInserir, tempoBusca, tempoRemover);
}
//Esse macro executa a adição, busca e remoção dos elementos na árvore em elementos fora de ordem
void macroSemOrdem(No* raiz, int tipo){
    clock_t inicio, fim;
    double tempoInserir, tempoRemover, tempoBusca;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO/2; i++){
        if(tipo == AVL){
            raiz = inserirAVL(raiz, i);
            raiz = inserirAVL(raiz, i + SIZEMACRO/2 + 1);
        }else {
            raiz = inserirLLRB(raiz, i);
            raiz = inserirLLRB(raiz, i + SIZEMACRO/2 + 1);
        }
    }
    
    fim = clock();
    //imprimirArvore(raiz, tipo, 0); //Remova o comentário dessa linha para visualizar árvore gerada pelo macro
    tempoInserir = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO/2; i++){
        buscar(raiz, i);
        buscar(raiz, i + SIZEMACRO/2 + 1);
    }
    
    fim = clock();
    tempoBusca = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    inicio = clock();
    
    for(int i = 0; i < SIZEMACRO/2; i++){
        if(tipo == AVL){
            raiz = removerAVL(raiz, i);
            raiz = removerAVL(raiz, i + SIZEMACRO/2 + 1);
        }else {
            raiz = removerLLRB(raiz, i);
            raiz = removerLLRB(raiz, i + SIZEMACRO/2 + 1);
        }
    }
    
    fim = clock();
    tempoRemover = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    
    printf("Tempo insercao: %fs; Tempo busca: %fs; Tempo remocao: %fs (Fora de ordem)\n", tempoInserir, tempoBusca, tempoRemover);
}

