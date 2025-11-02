/*
 * algoritmo_avancados.c
 *
 * Simula o mapa de uma mansão como uma árvore binária de salas.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Estrutura de uma sala (nó da árvore binária) */
typedef struct Sala {
    char *nome;
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Cria, de forma dinâmica, uma cópia da string nome */
static char *copiaString(const char *s) {
    if (!s) return NULL;
    size_t n = strlen(s) + 1;
    char *c = malloc(n);
    if (!c) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    memcpy(c, s, n);
    return c;
}

/*
 * criarSala()
 * Aloca dinamicamente um nó Sala com o nome informado.
 * Inicializa ponteiros esq e dir para NULL.
 */
Sala *criarSala(const char *nome) {
    Sala *nova = malloc(sizeof(Sala));
    if (!nova) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    nova->nome = copiaString(nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

/*
 * liberarMapa()
 * Libera memória de todas as salas da árvore (pós-ordem).
 */
void liberarMapa(Sala *raiz) {
    if (!raiz) return;
    liberarMapa(raiz->esq);
    liberarMapa(raiz->dir);
    free(raiz->nome);
    free(raiz);
}

/*
 * explorarSalas()
 * Permite a navegação do jogador pela árvore a partir da raiz.
 * Mostra opções claras (esquerda, direita, sair) e registra o caminho visitado.
 */
void explorarSalas(Sala *raiz) {
    if (!raiz) return;

    Sala *atual = raiz;
    char buf[64];

    /* vetor dinâmico para registrar nomes das salas visitadas */
    size_t cap = 8, n = 0;
    char **visitadas = malloc(cap * sizeof(char *));
    if (!visitadas) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        /* registrar visita */
        if (n >= cap) {
            cap *= 2;
            visitadas = realloc(visitadas, cap * sizeof(char *));
            if (!visitadas) {
                perror("realloc");
                exit(EXIT_FAILURE);
            }
        }
        visitadas[n++] = atual->nome;

        /* exibe sala atual */
        printf("\nVocê está em: %s\n", atual->nome);

        /* se for folha, finaliza exploração automaticamente */
        if (!atual->esq && !atual->dir) {
            printf("Esta sala não possui caminhos (folha). A exploração terminou.\n");
            break;
        }

        /* apresenta opções disponíveis */
        printf("Escolha uma opção:\n");
        if (atual->esq) printf("  e - Ir para a ESQUERDA (%s)\n", atual->esq->nome);
        if (atual->dir) printf("  d - Ir para a DIREITA (%s)\n", atual->dir->nome);
        printf("  s - Sair da exploração\n");
        printf("Opção: ");

        if (!fgets(buf, sizeof(buf), stdin)) {
            /* entrada encerrada inesperadamente */
            puts("\nEntrada encerrada. Saindo.");
            break;
        }

        /* interpreta primeira letra não-branco */
        char opc = '\0';
        for (size_t i = 0; buf[i] != '\0'; ++i) {
            if (!isspace((unsigned char)buf[i])) { opc = tolower((unsigned char)buf[i]); break; }
        }

        if (opc == 's') {
            puts("Exploração encerrada pelo jogador.");
            break;
        } else if (opc == 'e') {
            if (atual->esq) {
                atual = atual->esq;
                continue;
            } else {
                puts("Caminho para ESQUERDA indisponível. Tente outra opção.");
                continue;
            }
        } else if (opc == 'd') {
            if (atual->dir) {
                atual = atual->dir;
                continue;
            } else {
                puts("Caminho para DIREITA indisponível. Tente outra opção.");
                continue;
            }
        } else {
            puts("Opção inválida. Use 'e', 'd' ou 's'.");
            continue;
        }
    }

    /* exibe resumo das salas visitadas */
    printf("\nSalas visitadas (ordem):\n");
    for (size_t i = 0; i < n; ++i) {
        printf("  %zu. %s\n", i + 1, visitadas[i]);
    }

    free(visitadas);
}

/*
 * main()
 * Monta o mapa inicial da mansão (árvore binária criada automaticamente)
 * e inicia a exploração a partir do Hall de Entrada.
 */
int main(void) {
    /* Montagem manual do mapa (todos os nós alocados dinamicamente) */
    Sala *hall = criarSala("Hall de Entrada");

    hall->esq = criarSala("Sala de Estar");
    hall->dir = criarSala("Cozinha");

    hall->esq->esq = criarSala("Biblioteca");
    hall->esq->dir = criarSala("Sala de Jantar");

    hall->esq->esq->esq = criarSala("Jardim");    /* folha */
    /* hall->esq->dir (Sala de Jantar) é folha por omissão */

    hall->dir->esq = criarSala("Despensa");
    hall->dir->dir = criarSala("Corredor");

    hall->dir->dir->dir = criarSala("Quarto");    /* folha */

    printf("Bem-vindo à Mansão Misteriosa!\n");
    printf("Navegue a partir do Hall de Entrada.\n");

    explorarSalas(hall);

    liberarMapa(hall);
    return 0;
}