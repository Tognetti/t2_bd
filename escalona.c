#include <stdlib.h>
#include <stdio.h>

#include "grafo.h"
#include "transacao.h"

int main() {
    // int max = 64;
    // int n = 0;
    // Transacao *transacoes = (Transacao*) malloc(sizeof(Transacao) * max);

    // while (fscanf(stdin, "%d %d %c %c\n", &transacoes[n].tempo, &transacoes[n].id, &transacoes[n].operacao, &transacoes[n].atributo) == 4) {
    //     n++;
    //     if (n >= max) {
    //         max += 10;
    //         transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * max);
    //     }
    // }
    // transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * n);

    Graph* grafo = createGraph(3);
    addEdge(grafo, 0, 1);
    addEdge(grafo, 1, 2);
    addEdge(grafo, 2, 0);
    // addEdge(grafo, 1, 3);
    // addEdge(grafo, 1, 4);
    // addEdge(grafo, 2, 3);
    // addEdge(grafo, 3, 4);

    if(contemCiclo(grafo) == 1) {
        printf("Contém ciclo\n");
    } else {
        printf("Não contém ciclo\n");
    }

    //free(transacoes);
    return 0;
}