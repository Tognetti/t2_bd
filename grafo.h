// Struct que representa um nodo da lista de adjacência
struct AdjListNode {
    int dest;
    struct AdjListNode *next;
};

// Struct que representa uma lista de adjacência
struct AdjList {
    struct AdjListNode *head; //ponteiro para o nodo head da lista
};

// Struct que representa um grafo. Um grafo é um array de listas de adjacência
// Tamanho desse array é V (número de vértices no grafo)
struct Graph {
    int V;
    struct AdjList *array;
};

// Cria um nodo da lista de adjacência
struct AdjListNode* newAdjListNode(int dest);

// Cria um grafo com V vértices
struct Graph* createGraph(int V);

// Adiciona aresta ao grafo
void addEdge(struct Graph* graph, int src, int dest);

// Imprime grafo
void printGraph(struct Graph* graph);