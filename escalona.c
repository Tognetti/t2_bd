#include <stdlib.h>
#include <stdio.h>

#include "grafo.h"
#include "transacao.h"

int main() {

    // ***
    // Lê cada linha das transações
    // ***
    int max = 32;
    int n = 0;
    Transacao* transacoes = (Transacao*) malloc(sizeof(Transacao) * max);

    while (fscanf(stdin, "%d %d %c %c\n", &transacoes[n].tempo, &transacoes[n].id, &transacoes[n].operacao, &transacoes[n].atributo) == 4) {
        n++;
        if (n >= max) {
            max += 10;
            transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * max);
        }
    }
    transacoes = (Transacao*) realloc(transacoes, sizeof(Transacao) * n);

    // ***
    // Separa as transações nos respectivos escalonamentos
    // ***
    Escalonamento* escalonamentos = (Escalonamento*) malloc(sizeof(Escalonamento) * n);
    int escal_n = 0;

    for(int i = 0; i < n; i++) {
        escalonamentos[i].transacoes = (Transacao*) malloc(sizeof(Transacao) * n);
    }

    List* list = makelist(); //Lista utilizada para saber quais transações estão iniciadas

    Transacao* transacoes_temp = (Transacao*) malloc(sizeof(Transacao) * n);
    int transacoes_temp_n = 0;
    
    int cont_ids = 0;

    for(int i = 0; i < n; i++) {
        if((transacoes[i].operacao == 'R' || transacoes[i].operacao == 'W') && (find(transacoes[i].id, list) == 0)) {
            add(transacoes[i].id, list);
        } else if(transacoes[i].operacao == 'C' && (find(transacoes[i].id, list) == 1)) {
            delete(transacoes[i].id, list);
            cont_ids++;    
        }
        transacoes_temp[transacoes_temp_n] = transacoes[i];
        transacoes_temp_n++;

        // Se todas as transações atuais foram finalizadas, adiciona elas no escalonamento
        if(list->head == NULL) {
            for(int j = 0; j < transacoes_temp_n; j++) {
                escalonamentos[escal_n].transacoes[j] = transacoes_temp[j];
            }
            escalonamentos[escal_n].size = transacoes_temp_n;
            escalonamentos[escal_n].n_ids = cont_ids;
            transacoes_temp_n = 0;
            cont_ids = 0;
            escal_n++;   
        }
    }

    destroy(list);
    free(transacoes_temp);

    // ***
    // Cria um grafo para cada escalonamento
    // ***
    int grafos_n = escal_n;
    Graph** grafos = malloc(sizeof(Graph*) * grafos_n);

    for(int i = 0; i < grafos_n; i++) {
        grafos[i] = createGraph(escalonamentos[i].n_ids);
    }

    for(int i = 0; i < grafos_n; i++) {
        for(int j = 0; j < escalonamentos[i].size; j++) {
            Transacao t1 = escalonamentos[i].transacoes[j];

            for(int k = j + 1; k < escalonamentos[i].size; k++) {
                Transacao t2 = escalonamentos[i].transacoes[k];

                if(t1.atributo == t2.atributo && t1.id != t2.id) {
                    if(t1.operacao == 'W' && t2.operacao == 'R') {
                        addEdge(grafos[i], t1.id-1, t2.id-1);
                    }
                    if(t1.operacao == 'R' && t2.operacao == 'W') {
                        addEdge(grafos[i], t1.id-1, t2.id-1);
                    }
                    if(t1.operacao == 'W' && t2.operacao == 'W') {
                        addEdge(grafos[i], t1.id-1, t2.id-1);
                    }
                }
            }
        }
    }

    // ***
    // Verifica se escalonamento é serial e imprime os resultados
    // O escalonamento é serial se seu grafo correspondente não contém ciclos
    // ***
    for(int i = 0; i < escal_n; i++) {
        printf("%d ", i+1);
        List* lista_ids = makelist();

        for(int j = 0; j < escalonamentos[i].size; j++) {
            if(find(escalonamentos[i].transacoes[j].id, lista_ids) == 0) {
                add(escalonamentos[i].transacoes[j].id, lista_ids);
            }
        }
        display(lista_ids);
        if(contemCiclo(grafos[i]) == 1) {
            printf("NS ");
            // Se não existir blind write, então escalonamento também não vai ser equivalente por visão
            int aux = 0; // Assumo que não existem blind writes no escalonamento

            for(int j = 0; j < escalonamentos[i].size && aux == 0; j++) {
                Transacao t1 = escalonamentos[i].transacoes[j];
                if(t1.operacao == 'W') {
                    int blind_write = 1;
                    for(int k = 0; k < j; k++) {
                        Transacao t2 = escalonamentos[i].transacoes[k];
                        if((t1.id == t2.id) && (t2.operacao == 'R') && (t1.atributo == t2.atributo)) {
                            blind_write = 0;
                        }
                    }
                    if(blind_write == 1) {
                        // Como existe um blind write, ainda devo verificar
                        aux = 1;
                        printf("Existe blind write, ainda devo verificar\n");

                        // Gero todos os escalonamentos seriais possíveis com base no escalonamento original
                        int permutacoes_esc_n;
                        Escalonamento* permutacoes_esc = geraPermutacoes(escalonamentos[i], &permutacoes_esc_n);

                        // TODO: Preciso comparar o escalonamento original com todos os que gerei, verificando condições
                        int visao_equivalente = verificaVisaoEquivalente(escalonamentos[i], permutacoes_esc, permutacoes_esc_n);                 
                    }
                }
            }
            
            if(aux == 0) {
                //Não contém blind write
                printf("NV");
            }

        } else {
            printf("SS SV"); // Todo escalonamento serial por conflito também é equivalente por visão
        }
        printf("\n");
        destroy(lista_ids);
    }

    free(transacoes);

    for(int i = 0; i < grafos_n; i++) {
        destroyGraph(grafos[i]);
    }
    free(grafos);
    
    for(int i = 0; i < n; i++) {
        free(escalonamentos[i].transacoes);
    }
    free(escalonamentos);

    return 0;
}