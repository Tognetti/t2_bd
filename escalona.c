#include <stdlib.h>
#include <stdio.h>

#include "grafo.h"
#include "transacao.h"

int main() {
    // addEdge(graph, 0, 1);
    // addEdge(graph, 0, 4);
    // addEdge(graph, 1, 2);
    // addEdge(graph, 1, 3);
    // addEdge(graph, 1, 4);
    // addEdge(graph, 2, 3);
    // addEdge(graph, 3, 4);
    // printGraph(graph);

    int max = 64;
    int n = 0;
    Transacao *transacoes = (Transacao*) malloc(sizeof(Transacao) * max);

    while (fscanf(stdin, "%d %d %c %c\n", &transacoes[n].tempo, &transacoes[n].id, &transacoes[n].operacao, &transacoes[n].atributo) == 4) {
        n++;
        if (n >= max) {
            max += 10;
            transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * max);
        }
    }
    transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * n);

    for(int i = 0; i < n; i++) {
        printf("%d\n", transacoes[i].id);
    }

    struct Graph* graph = createGraph(n);

    return 0;
}