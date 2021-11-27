#ifndef __KNAPSACK__
#define __KNAPSACK__

#include "global_vars.h"

float rational_knapsack (product_t *items, int n_items, int ks_cap);
void naive_knapsack (int l);
void branch_bound_knapsack (int l, int cur_w);

#endif