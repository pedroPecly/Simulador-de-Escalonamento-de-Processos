#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct _processo {
    char nome[50];
    float tempo_de_execucao;
    struct _processo *proximo;
} Processo;

typedef struct _fila {
    char nome[50];
    Processo *inicio;
    Processo *final;
} Fila;

Processo *criarProcesso(char nome[], float tempo_de_execucao) {
    Processo *novoProcesso = (Processo *)malloc(sizeof(Processo));
    if (novoProcesso != NULL) {
        strcpy(novoProcesso->nome, nome);
        novoProcesso->tempo_de_execucao = tempo_de_execucao;
        novoProcesso->proximo = NULL;
    }
    return novoProcesso;
}

Fila *criarFila(char nome[]) {
    Fila *novaFila = (Fila *)malloc(sizeof(Fila));
    strcpy(novaFila->nome, nome);
    novaFila->inicio = novaFila->final = NULL;
    return novaFila;
}

void enfileirar(Fila *f, Processo *novo) {
    if (novo != NULL) {
        if (f->inicio == NULL) {
            f->inicio = f->final = novo;
        } else {
            f->final = f->final->proximo = novo;
        }
    }
}

Processo *desenfileirar(Fila *f) {
    if (f->inicio == NULL) {
        return NULL;
    }
    Processo *aux = f->inicio;
    f->inicio = aux->proximo;
    aux->proximo = NULL;
    return aux;
}

void liberarFila(Fila *f) {
    Processo *aux = f->inicio;
    while (aux != NULL) {
        Processo *aux2 = aux->proximo;
        free(aux);
        aux = aux2;
    }
    free(f);
}

short int filaEstaVazia(Fila *f) {
    return (f->inicio == NULL);
}

void moverProcessos(Fila *origem, Fila *destino) {
    while (!filaEstaVazia(origem)) {
        Processo *aux = desenfileirar(origem);
        enfileirar(destino, aux);
    }
}

void lerArquivoProcessos(FILE *arq, Fila *f1) {
    for (int i = 0; !feof(arq); i++) {
        char nome[20];
        float tempo_de_execucao;

        fscanf(arq, "%s %f\n", nome, &tempo_de_execucao);
        Processo *novo = criarProcesso(nome, tempo_de_execucao);

        enfileirar(f1, novo);
    }
}

void imprimirElementos(Fila *f) {
    printf("%s: ", f->nome);
    Processo *aux = f->inicio;
    if (aux != NULL) {
        while (aux != NULL) {
            if (aux->proximo == NULL) {
                printf("%s = %.f\n", aux->nome, aux->tempo_de_execucao);
            } else {
                printf("%s = %.f <- ", aux->nome, aux->tempo_de_execucao);
            }
            aux = aux->proximo;
        }
    } else {
        printf("vazia\n");
    }
}

int main() {
    FILE *arq;
    char caminho[100];
    char linha[50];
    int quantum;

    Fila *pronto = criarFila("Pronto");
    Fila *espera = criarFila("Espera");
    Fila *execucao = criarFila("Execucao");
    Fila *finalizado = criarFila("Finalizados");

    scanf("%s", caminho);

    arq = fopen(caminho, "rt");
    if (arq != NULL) {

        fscanf(arq, "%d", &quantum);
        while (!feof(arq)) {
            if (fgets(linha, 50, arq) != NULL) {
                lerArquivoProcessos(arq, pronto);
            }
        }

        fclose(arq);

        printf("Estado atual das filas de processos:\n");
        imprimirElementos(pronto);
        imprimirElementos(espera);
        imprimirElementos(execucao);
        imprimirElementos(finalizado);
        printf("\n");
        printf("Quantum: %d\n", quantum);
        printf("\n");
        printf("----------> Iniciando execucao\n");

        while (!filaEstaVazia(pronto)) {
            moverProcessos(pronto, execucao);
            printf("Processos foram adicionados a fila de execucao\n");

            while (!filaEstaVazia(execucao)) {
                printf("Estado atual das filas de processos:\n");
                imprimirElementos(pronto);
                imprimirElementos(espera);
                imprimirElementos(execucao);
                imprimirElementos(finalizado);
                Processo *aux = desenfileirar(execucao);

                aux->tempo_de_execucao -= quantum;
                printf("%s esta executando...\n", aux->nome);

                usleep(quantum);

                if (aux->tempo_de_execucao > 0) {
                    printf("Quantum expirou, %s sofreu preempcao\n\n", aux->nome);
                    enfileirar(espera, aux);
                    printf("%s foi adicionado a fila de espera\n", aux->nome);
                    printf("\n-------------------------------------\n");
                } else {
                    aux->tempo_de_execucao = 0;
                    enfileirar(finalizado, aux);
                    printf("%s terminou a execucao, %s foi adicionado a fila de "
                        "finalizados\n\n",
                        aux->nome, aux->nome);
                    printf("-------------------------------------\n");
                }
            }

            if (!filaEstaVazia(espera)) {
                printf("A fila de execucao esta vazia, movendo processos para a fila "
                    "de prontos\n");
                moverProcessos(espera, pronto);
                printf("Estado atual das filas de processos:\n");
                imprimirElementos(pronto);
                imprimirElementos(espera);
                imprimirElementos(execucao);
                imprimirElementos(finalizado);
                printf("\n-------------------------------------\n");
            }
        }
        printf("Estado atual das filas de processos:\n");
        imprimirElementos(pronto);
        imprimirElementos(espera);
        imprimirElementos(execucao);
        imprimirElementos(finalizado);
        printf("Nao ha mais processos a serem executados\n");
        printf("----------> Simulacao finalizada\n");

        liberarFila(pronto);
        liberarFila(espera);
        liberarFila(execucao);
        liberarFila(finalizado);

        return 0;
    } else {
        printf("Arquivo nao encontrado.\n");
        return 1;
    }
}
