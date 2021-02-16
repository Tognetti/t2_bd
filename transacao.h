typedef struct transacao {
    int tempo; 
    int id;
    char operacao;
    char atributo;
} Transacao;

int leTransacoes(Transacao *transacoes);