#include "linkedlist.h"

// Struct que representa um nodo da lista de adjacência
typedef struct AdjListNode {
    int valor;
    struct AdjListNode *next;
} AdjListNode;

// Struct que representa uma lista de adjacência
typedef struct AdjList {
    int valor;
    AdjListNode *head; //ponteiro para o nodo head da lista
} AdjList;

// Struct que representa um grafo. Um grafo é um array de listas de adjacência
// Tamanho desse array é V (número de vértices no grafo)
typedef struct Graph {
    int V;
    AdjList *array;
} Graph;

// Cria um nodo da lista de adjacência
AdjListNode* newAdjListNode(int dest);

// Cria um grafo com V vértices
Graph* createGraph(int V);

// Adiciona aresta ao grafo
void addEdge(Graph* graph, int src, int dest);

// Imprime grafo
void printGraph(Graph* graph);

int contemCiclo(Graph* graph);

int DFS(Graph* grafo, int v, int* visitados, int* visitados_n, List* callStack);