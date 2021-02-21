#include <stdio.h>
#include <stdlib.h>

#include "grafo.h"
#include "linkedlist.h"

// A utility function to create a new adjacency list node
AdjListNode* newAdjListNode(int valor) {
    AdjListNode* newNode = (AdjListNode*) malloc(sizeof(AdjListNode));
    newNode->valor = valor;
    newNode->next = NULL;
    return newNode;
}

// A utility function that creates a graph of V vertices
Graph* createGraph(int V) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->V = V;

    // Create an array of adjacency lists.  Size of array will be V
    graph->array = (AdjList*) malloc(V * sizeof(AdjList));

    // Initialize each adjacency list as empty by making head NULL
    int i;
    for (i = 0; i < V; ++i) {
        graph->array[i].head = NULL;
        graph->array[i].valor = i;
    }
    return graph;
}

// Adds an edge to graph
void addEdge(Graph* graph, int src, int dest) {
    // Add an edge from src to dest. A new node is added to the adjacency list of src. The node is added at the begining
    if(findEdge(graph, src, dest) == 0) { // Verifica se aresta já existe
        AdjListNode* newNode = newAdjListNode(dest);
        newNode->next = graph->array[src].head;
        graph->array[src].head = newNode;
    }   
}

int findEdge(Graph* graph, int src, int dest) {
    for (int v = 0; v < graph->V; v++) {
        AdjListNode* pCrawl = graph->array[v].head;
        while (pCrawl) {
            if(graph->array[v].valor == src && pCrawl->valor == dest) {
                return 1;
            }
            pCrawl = pCrawl->next;
        }
    }
    return 0;
}

void printGraph(Graph* graph) {
    for (int v = 0; v < graph->V; v++) {
        AdjListNode* pCrawl = graph->array[v].head;
        printf("\n Adjacency list of vertex %d\n head ", graph->array[v].valor);
        while (pCrawl) {
            printf("-> %d", pCrawl->valor);
            pCrawl = pCrawl->next;
        }
        printf("\n");
    }
}

void destroyGraph(Graph* graph) {
    for (int v = 0; v < graph->V; v++) {
        AdjListNode* pCrawl = graph->array[v].head;
        AdjListNode* next = pCrawl;
        while (pCrawl) {
            next = pCrawl->next;
            free(pCrawl);
            pCrawl = next;
        }
    }
    free(graph->array);
    free(graph);
}

int contemCiclo(Graph* graph) {
    int max = graph->V;
    int contem = 0;

    for (int v = 0; v < graph->V; v++) {
        if(contem == 0) {
            int* visitados = (int*) malloc(sizeof(int) * max);
            int visitados_n = 0;

            List* list = makelist();
            add(graph->array[v].valor, list);
            contem = DFS(graph, v, visitados, &visitados_n, list);

            free(visitados);
            destroy(list);
        } else {
            return contem;
        }

    }

    return contem;
}

int DFS(Graph* grafo, int v, int* visitados, int* visitados_n, List* callStack) {   
    int ciclo = 0;
    visitados[*visitados_n] = grafo->array[v].valor;   
    (*visitados_n)++;
    AdjListNode* nodo = grafo->array[v].head;
    int valor = grafo->array[v].valor;

    while(nodo) {
        for (int i = 0; i < grafo->V; i++) {
            if(grafo->array[i].valor == nodo->valor) {
                int visitado = 0;
                for(int j = 0; j < *visitados_n; j++) {
                    if(visitados[j] == nodo->valor) {
                        visitado = 1;
                    }
                }
                if(find(nodo->valor, callStack) == 0) {
                    if(visitado == 0) {
                        add(grafo->array[i].valor, callStack);
                        ciclo = DFS(grafo, i, visitados, visitados_n, callStack);                   
                    }  
                } else {
                    return 1;
                }
            }        
        }  
        nodo = nodo->next; 
    }

    delete(valor, callStack);
    
    return ciclo;
}