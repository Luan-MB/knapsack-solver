#ifndef __KNAPSACK__
#define __KNAPSACK__

typedef struct {
    int profit;
    int weight;
    int have_restrictions;
} product_t;

typedef struct {
    int r1;
    int r2;
} restriction_t;

void solve_knapsack ();

#endif