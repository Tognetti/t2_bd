typedef struct transacao {
    int tempo; 
    int id;
    char operacao;
    char atributo;
} Transacao;

typedef struct escalonamento {
    Transacao* transacoes;
    int size;
    int n_ids;
} Escalonamento;

Escalonamento* geraPermutacoes(Escalonamento e, int* tamanho);