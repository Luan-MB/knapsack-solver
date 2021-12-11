#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "knapsack.h"
#include "global_vars.h"

product_t *prods;
restriction_t *restrs;
int n_prods, pro_pairs, ks_cap, opt_p = 0, node = 0, cortes = 0;
int *X, *opt_x;

// Ordena o vetor de produtos em ordem crescente
static int profit_weight(const void * a, const void * b) {

    product_t *product1 = (product_t *) a;
    product_t *product2 = (product_t *) b;

    if (((float) product1->profit/product1->weight) < ((float) product2->profit/product2->weight)) return 1;
    if (((float) product1->profit/product1->weight) == ((float) product2->profit/product2->weight)) return 0;
    return -1;
}

// Funcao que checa se existem produtos conflitantes com prods[l] na solucao parcial
static int conflict(int l) {

    if (prods[l].have_restrictions) {
        for (int i=0; i<pro_pairs; ++i) {
            if (restrs[i].r1-1 == l)
                if ((restrs[i].r2-1 < l) && X[restrs[i].r2-1])
                    return 1;
            if (restrs[i].r2-1 == l)
                if ((restrs[i].r1-1 < l) && X[restrs[i].r1-1])
                    return 1;
        }
    }
    return 0;
}

// Funcao que calcula o caso otimo da mochila por meio de uma greedy strategy
// O retorno será utilizado como bound
static float rational_knapsack (product_t *prods, int n_prods, int ks_cap) {

    product_t aux[n_prods];
    memcpy (aux, prods, sizeof(product_t) * n_prods);

    // Ordena a copia do vetor de produtos em ordem crescente por profit/weight
    qsort(aux, n_prods, sizeof(product_t), profit_weight);

    int i = 0, W = 0;
    float P = 0;
    float x[n_prods];

    // Inicia o vetor solucao com todos os valores 0
    for (int j=0; j<n_prods; ++j)
        x[j] = 0;
    
    // Enquanto o peso nao for menor que o limite
    // E existirem prods a serem inseridos
    while(W < ks_cap && i < n_prods) {
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

/*
// Resolve o problema da mochila pelo metodo ingenuo
static void naive_knapsack (int l) {

    if (l == n_prods) {
        int total_weight = 0;
        for (int i=0; i<n_prods; ++i)
            total_weight += prods[i].weight * X[i];
        if (total_weight <= ks_cap) {
            int cur_p = 0;
            for (int j=0; j<n_prods; ++j)
                cur_p += prods[j].profit * X[j];
            if (cur_p > opt_p) {
                for (int h=0; h<pro_pairs; ++h)
                    if (X[restrs[h].r1-1] && X[restrs[h].r2-1])
                        return;
                for (int k=0; k<n_prods; ++k)
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
*/

// Resolve o problema da mochila por branch and bound
static void branch_bound_knapsack (int l, int cur_w) {

    // Caso o nodo for folha, calcula se a solucao encontrada e malhor que a otima ate o momento
    if (l == n_prods) {
        int total_profit = 0;
        for (int i=0; i<n_prods; ++i)
            total_profit += prods[i].profit * X[i];
        if (total_profit > opt_p) { // Se for melhor, ela passa a ser a nova otima
            for (int k=0; k<n_prods; ++k)
                opt_x[k] = X[k];
            opt_p = total_profit;
        }
        
    } else {
        node++;
        // Calcula a nova bound a partir da: 
        // Solucao parcial + solucao otima pelo metodo greedy do vetor de produtos restantes
        int partial_solution = 0;
        for (int i=0; i<l; ++i)
            partial_solution += prods[i].profit * X[i];
        float bound = partial_solution + rational_knapsack(prods+l, n_prods-l, ks_cap - cur_w);
        // Checa se a subarvore do nodo e candidata a melhorar a solucao parcial
        if (bound <= opt_p) {
            cortes++;
            return;
        }
        // Analisa se o produto a ser inserido esta dentro do limite de peso
        // E se atende as regras de conflito entre os produtos
        if ((cur_w + prods[l].weight <= ks_cap) && !(conflict(l))) {
            X[l] = 1;                                                   // Testa-se o nodo com valor 1
            branch_bound_knapsack(l+1, cur_w + prods[l].weight * X[l]);
        }
        X[l] = 0;                                                       // Testa-se o nodo com valor 0
        branch_bound_knapsack(l+1, cur_w + prods[l].weight * X[l]);
    }
}

void solve_knapsack () {
    //naive_knapsack(0);
    branch_bound_knapsack(0, 0);
}
