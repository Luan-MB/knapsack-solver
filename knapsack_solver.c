#include <stdio.h>
#include <stdlib.h>
#include "global_vars.h"
#include "knapsack.h"

product_t *prods;
restriction_t *restrs;
int n_prods, pro_pairs, ks_cap, opt_p, node, cortes;
int *X, *opt_x;

int main () {

    scanf("%d %d %d", &n_prods, &pro_pairs, &ks_cap);

    prods = malloc (n_prods * sizeof(product_t));
    restrs = malloc (pro_pairs * sizeof(restriction_t));
    X = malloc(n_prods * sizeof(int));
    opt_x = malloc(n_prods * sizeof(int));


    for (int i=0; i<n_prods; ++i)
        scanf("%d", &prods[i].weight);
    for (int i=0; i<n_prods; ++i) {
        scanf("%d", &prods[i].profit);
        prods[i].have_restrictions = 0;
    }

    for (int i=0; i<pro_pairs; ++i) {
        scanf("%d %d", &restrs[i].r1, &restrs[i].r2);
        if (!(prods[restrs[i].r1-1].have_restrictions))
            prods[restrs[i].r1-1].have_restrictions = 1;
        if (!(prods[restrs[i].r2-1].have_restrictions))
            prods[restrs[i].r2-1].have_restrictions = 1;
    }

    set_timer();

    solve_knapsack();
    
    fprintf(stderr, "nodes analised: %d, discarded branches %d, runtime: %d ms\n", node, cortes, global_time);

    printf("%d\n", opt_p);
    for (int i=0; i<n_prods; ++i)
        if (opt_x[i] == 1)
            printf("%d ", i+1);
    printf("\n");

    free(prods);
    free(restrs);
    free(X);
    free(opt_x);

    return 0;
}