#include <stdio.h>
#include <stdlib.h>

#include "transacao.h"

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void printarray(int arr[], int size)
{
    int i,j;
    for(i=0; i<size; i++)
    {
        printf("%d\t",arr[i]);
    }
    printf("\n");
}

void permutaIds(int* arr, int start, int end, int** permutacoes, int n, int* m) {
    if(start==end) {
        permutacoes[*m] = (int*) malloc(sizeof(int) * n);
        for(int i = 0; i < n; i++) {
            permutacoes[*m][i] = arr[i];
        }
        (*m)++;      
        return;
    }

    int i;
    for(i=start;i<=end;i++) {
        swap((arr+i), (arr+start));
        permutaIds(arr, start+1, end, permutacoes, n, m);
        swap((arr+i), (arr+start));
    }
}

Escalonamento* geraPermutacoes(Escalonamento e, int* tamanho) {

    // Armazena os ids das transações em um array
    int* ids = malloc(sizeof(int) * e.n_ids);
    int ids_n = 0;

    for(int i = 0; i < e.size; i++) {
        int id_presente = 0;
        for(int j = 0; j < ids_n; j++) {
            if(ids[j] == e.transacoes[i].id) {
                id_presente = 1;
            }
        }

        if(id_presente == 0) {
            ids[ids_n] = e.transacoes[i].id;
            ids_n++;
        }
    }

    int n = ids_n;
    int fat = 1;
    for(fat; n > 1; n = n - 1) {
      fat = fat * n;
    }

    int permutacoes_n = fat;
    int** permutacoes = (int **) malloc(sizeof(int*) * permutacoes_n);
    int m = 0;

    permutaIds(ids, 0, ids_n-1, permutacoes, ids_n, &m);

    Escalonamento* escalonamentos = (Escalonamento*) malloc(sizeof(Escalonamento) * permutacoes_n);
    int escalonamentos_n = 0;

    for(int i = 0; i < permutacoes_n; i++) {
        escalonamentos[i].transacoes = (Transacao*) malloc(sizeof(Transacao) * e.size);
        escalonamentos[i].size = e.size;
        escalonamentos[i].n_ids = e.n_ids;
    }
    
    for(int i = 0; i < permutacoes_n; i++) {
        int cont_t = 0;
        for(int j = 0; j < ids_n; j++) {   
            for(int k = 0; k < e.size; k++) {
                if((e.transacoes[k].id == permutacoes[i][j])) {
                    escalonamentos[escalonamentos_n].transacoes[cont_t].id = e.transacoes[k].id;
                    escalonamentos[escalonamentos_n].transacoes[cont_t].operacao = e.transacoes[k].operacao;
                    escalonamentos[escalonamentos_n].transacoes[cont_t].atributo = e.transacoes[k].atributo;
                    escalonamentos[escalonamentos_n].transacoes[cont_t].tempo = cont_t+1;
                    cont_t++;
                }
            }  
        }
        escalonamentos_n++;
    }
    *tamanho = escalonamentos_n;
    return escalonamentos;
}

Transacao* encontraFinalWrites(Escalonamento e, int* n) {
    Transacao* final_writes = (Transacao*) malloc(sizeof(Transacao) * e.size);
    int final_writes_n = 0;

    for(int a = e.size-1; a >= 0; a--) {
        Transacao t = e.transacoes[a];
        if(t.operacao == 'W') {
            int aux = 0;
            for(int b = 0; b < final_writes_n; b++) {
                if(final_writes[b].atributo == t.atributo) {
                    aux = 1;
                }
            }
            if(aux == 0) {
                // Ainda não achei um final write para esse atributo, então adiciono no array
                final_writes[final_writes_n] = t;
                final_writes_n++;
            }
        }
    }

    *n = final_writes_n;
    return final_writes;
}

int verificaVisaoEquivalente(Escalonamento e, Escalonamento* permutacoes, int permutacoes_n) {

    printf("Escalonamento original\n");

    // Encontra as últimas operações de escrita em cada atributo do escalonamento original
    int final_writes_n;
    Transacao* final_writes = encontraFinalWrites(e, &final_writes_n);

    for(int i = 0; i < final_writes_n; i++) {
        printf("%d %d %c %c\n", final_writes[i].tempo, final_writes[i].id, final_writes[i].operacao, final_writes[i].atributo);
    }

    for(int i = 0; i < permutacoes_n; i++) {
        Escalonamento esc = permutacoes[i];

        Transacao* final_writes_permutacao = encontraFinalWrites(esc, &final_writes_n);
        int aux = 0;
        printf("Final writes do escalonamento %d\n", i);
        for(int j = 0; j < final_writes_n && aux == 0; j++) {
            printf("%d %d %c %c\n", final_writes_permutacao[j].tempo, final_writes_permutacao[j].id, final_writes_permutacao[j].operacao, final_writes_permutacao[j].atributo);
            for(int k = 0; k < final_writes_n; k++) {
                if(final_writes_permutacao[j].atributo == final_writes[k].atributo) {
                    if(final_writes_permutacao[j].id != final_writes[k].id) {
                        aux = 1;
                    }   
                }
            }
        }
        if(aux == 0) {
            printf("Permutação tem os final writes iguais\n");
        } else {
            printf("Permutação não tem os final writes iguais\n");
        }
    }


    // for(int i = 0; i < permutacoes_n; i++) {
    //     Escalonamento esc = permutacoes[i];
    //     printf("Escalonamento %d\n", i);
    //     for(int j = 0; j < esc.size; j++) {
    //         printf("%d %d %c %c\n", esc.transacoes[j].tempo, esc.transacoes[j].id, esc.transacoes[j].operacao, esc.transacoes[j].atributo);
    //     }
    //     printf("\n");
    // }  
}

