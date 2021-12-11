#ifndef __GLOBAL_VARS__
#define __GLOBAL_VARS__

#include "knapsack.h"

extern void set_timer();

extern product_t *prods;
extern restriction_t *restrs;
extern int n_prods;
extern int pro_pairs;
extern int ks_cap;
extern int opt_p;
extern int node;
extern int cortes;
extern int *X;
extern int *opt_x;
extern unsigned global_time;

#endif