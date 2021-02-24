#include <stdio.h>
#include <stdlib.h>

#include "transacao.h"

void swap(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

// Recebe um array de int (arr) e retorna todas as permutações possíveis no array permutacoes
void permutaIds(int* arr, int start, int end, int** permutacoes, int n, int* m) {
    if(start == end) {
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

// Recebe um escalonamento e retorna as permutações possíveis de escalonamentos seriais
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

    // Calcula o número de permutações possíveis
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

// Retorna as últimas transações que fazem operação de W em cada atributo
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
                // Ainda não tenho um final write para esse atributo, então adiciono no array
                final_writes[final_writes_n] = t;
                final_writes_n++;
            }
        }
    }

    *n = final_writes_n;
    return final_writes;
}

// Verifica a seguinte condição entre dois escalonamentos:
// Para cada r(x) de Ti, se o valor de x lido for escrito por w(x) de Tj, o mesmo deve permanecer para r(x) de Ti em S'
int verificaWriteBeforeReadIguais(Escalonamento e_permutacao, Escalonamento e_original) {

    // Encontra cada operação de R na permutação e o respectivo W que escreveu o atributo
    for(int j = 0; j < e_permutacao.size; j++) {
        Transacao t_r = e_permutacao.transacoes[j];
        Transacao t_w;
        
        if(t_r.operacao == 'R') {
            int found = 0;
            for(int k = j-1; k >= 0 && found == 0; k--) {
                t_w = e_permutacao.transacoes[k];
                if(t_w.operacao == 'W' && (t_r.atributo == t_w.atributo)) {
                    found = 1;
                }
            }

            // Compara se no escalonamento original o W que escreveu para o R é o mesmo
            for(int k = 0; k < e_original.size; k++) {
                Transacao t_r_original = e_original.transacoes[k];
                if((t_r_original.operacao == 'R') && (t_r_original.atributo == t_r.atributo) && (t_r_original.id == t_r.id)) {
                    int found2 = 0;
                    for(int l = k-1; l >= 0 && found2 == 0; l--) {
                        Transacao t_w_original = e_original.transacoes[l];
                        if((t_w_original.operacao == 'W') && (t_w_original.atributo == t_r_original.atributo) && (t_w_original.id == t_w.id)) {
                            found2 = 1;
                        }                               
                    }

                    if(found2 == 0 && found == 1) {
                        return 0;
                    }
                }
            }
        }
    }
    return 1;
}

int verificaVisaoEquivalente(Escalonamento e, Escalonamento* permutacoes, int permutacoes_n) {

    // Encontra as últimas operações de escrita em cada atributo do escalonamento original
    int final_writes_n;
    Transacao* final_writes = encontraFinalWrites(e, &final_writes_n);

    for(int i = 0; i < permutacoes_n; i++) {
        Escalonamento esc = permutacoes[i];
        int equivalente = 0;

        
        // Verifica condição:
        // Se o operador w(y) em Tk é a ultima escrita de y em S, então w(y) em Tk deve ser a última escrita em S' 
        Transacao* final_writes_permutacao = encontraFinalWrites(esc, &final_writes_n);
        int aux = 0;
        for(int j = 0; j < final_writes_n && aux == 0; j++) {
            for(int k = 0; k < final_writes_n; k++) {
                if(final_writes_permutacao[j].atributo == final_writes[k].atributo) {
                    if(final_writes_permutacao[j].id != final_writes[k].id) {
                        aux = 1;
                    }   
                }
            }
        }
        if(aux == 0) {
            return verificaWriteBeforeReadIguais(esc, e);
        }
    }
    return 0;
}

