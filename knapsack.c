#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "knapsack.h"

product_t *items;
danger_t *dangers;
int n_items, pro_pairs, ks_cap, opt_p = 0, node = 1;
int *X, *opt_x;

// Ordena o vetor de produtos em ordem crescente
static int profit_weight(const void * a, const void * b) {

    product_t *product1 = (product_t *) a;
    product_t *product2 = (product_t *) b;

    if (((float) product1->profit/product1->weight) < ((float) product2->profit/product2->weight)) return 1;
    if (((float) product1->profit/product1->weight) == ((float) product2->profit/product2->weight)) return 0;
    return -1;
}

// Funcao que checa se existem produtos conflitantes com items[l] na solucao parcial
static int conflict(int l) {

    if (items[l].have_restrictions) {
        for (int i=0; i<pro_pairs; ++i) {
            if (dangers[i].r1-1 == l)
                if ((dangers[i].r2-1 < l) && X[dangers[i].r2-1])
                    return 1;
            if (dangers[i].r2-1 == l)
                if ((dangers[i].r1-1 < l) && X[dangers[i].r1-1])
                    return 1;
        }
    }
    return 0;
}

// Funcao que calcula o caso otimo da mochila por meio de uma greedy strategy
// O retorno será utilizado como bound
float rational_knapsack (product_t *items, int n_items, int ks_cap) {

    product_t aux[n_items];
    memcpy (aux, items, sizeof(product_t) * n_items);

    // Ordena a copia do vetor de produtos em ordem crescente por profit/weight
    qsort(aux, n_items, sizeof(product_t), profit_weight);

    int i = 0, W = 0;
    float P = 0;
    float x[n_items];

    // Inicia o vetor solucao com todos os valores 0
    for (int j=0; j<n_items; ++j)
        x[j] = 0;
    
    // Enquanto o peso nao for menor que o limite
    // E existirem items a serem inseridos
    while(W < ks_cap && i < n_items) {
        // Se o item couber na mochila ele e inserido
        if ((W + aux[i].weight) <= ks_cap) {
            x[i] = 1;
            W += aux[i].weight;
            P += aux[i].profit;
            i++;
        // Se nao insere-se apenas a porcentagem de seu peso que ainda cabe
        } else {
            x[i] = (float) (ks_cap - W)/aux[i].weight;
            W = ks_cap;
            P += (float) x[i]*aux[i].profit;
            i++;
        }

    }
    return P;
}

// Resolve o problema da mochila pelo metodo ingenuo
void naive_knapsack (int l) {

    if (l == n_items) {
        int total_weight = 0;
        for (int i=0; i<n_items; ++i)
            total_weight += items[i].weight * X[i];
        if (total_weight <= ks_cap) {
            int cur_p = 0;
            for (int j=0; j<n_items; ++j)
                cur_p += items[j].profit * X[j];
            if (cur_p > opt_p) {
                for (int h=0; h<pro_pairs; ++h)
                    if (X[dangers[h].r1-1] && X[dangers[h].r2-1])
                        return;
                for (int k=0; k<n_items; ++k)
                    opt_x[k] = X[k];
                opt_p = cur_p;
            }
        }

    } else {
        node++;
            X[l] = 1;
            naive_knapsack(l+1);
            X[l] = 0;
            naive_knapsack(l+1);
    }
}

// Resolve o problema da mochila por branch and bound
void branch_bound_knapsack (int l, int cur_w) {

    // Caso o nodo for folha, calcula se a solucao encontrada e malhor que a otima ate o momento
    if (l == n_items) {
        int total_profit = 0;
        for (int i=0; i<n_items; ++i)
            total_profit += items[i].profit * X[i];
        if (total_profit > opt_p) { // Se for melhor, ela passa a ser a nova otima
            for (int k=0; k<n_items; ++k)
                opt_x[k] = X[k];
            opt_p = total_profit;
        }
        
    } else {
        node++;
        // Calcula a nova bound a partir da: 
        // Solucao parcial + solucao otima pelo metodo greedy do vetor de produtos restantes
        int partial_solution = 0;
        for (int i=0; i<l; ++i)
            partial_solution += items[i].profit * X[i];
        float bound = partial_solution + rational_knapsack(items+l, n_items-l, ks_cap - cur_w);
        // Checa se a subarvore do nodo e candidata a melhorar a solucao parcial
        if (bound <= opt_p) return;
        // Analisa se o produto a ser inserido esta dentro do limite de peso
        // E se atende as regras de conflito entre os produtos
        if ((cur_w + items[l].weight <= ks_cap) && !(conflict(l))) {
            X[l] = 1;                                                   // Testa-se o nodo com valor 1
            branch_bound_knapsack(l+1, cur_w + items[l].weight * X[l]);
        }
        X[l] = 0;                                                       // Testa-se o nodo com valor 0
        branch_bound_knapsack(l+1, cur_w + items[l].weight * X[l]);
    }
}
