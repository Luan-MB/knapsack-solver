#include <stdio.h>
#include <stdlib.h>
#include "global_vars.h"
#include "knapsack.h"

product_t *items;
danger_t *dangers;
int n_items, pro_pairs, ks_cap, opt_p, node;
int *X, *opt_x;
float bound;

int main () {

    scanf("%d %d %d", &n_items, &pro_pairs, &ks_cap);

    items = malloc (n_items * sizeof(product_t));
    dangers = malloc (pro_pairs * sizeof(danger_t));
    X = malloc(n_items * sizeof(int));
    opt_x = malloc(n_items * sizeof(int));


    for (int i=0; i<n_items; ++i)
        scanf("%d", &items[i].weight);
    for (int i=0; i<n_items; ++i) {
        scanf("%d", &items[i].profit);
        items[i].have_restrictions = 0;
    }

    for (int i=0; i<pro_pairs; ++i) {
        scanf("%d %d", &dangers[i].r1, &dangers[i].r2);
        if (!(items[dangers[i].r1-1].have_restrictions))
            items[dangers[i].r1-1].have_restrictions = 1;
        if (!(items[dangers[i].r2-1].have_restrictions))
            items[dangers[i].r2-1].have_restrictions = 1;
    }

    set_timer();
    //naive_knapsack(0);
    branch_bound_knapsack(0, 0);
    
    fprintf(stderr, "nodes: %d, time: %d ms\n", node, global_time);
    
    printf("%d\n", opt_p);
    for (int i=0; i<n_items; ++i)
        if (opt_x[i] == 1)
            printf("%d ", i+1);
    printf("\n");

    free(items);
    free(dangers);
    free(X);
    free(opt_x);

    return 0;
}