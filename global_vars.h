#ifndef __GLOBAL_VARS__
#define __GLOBAL_VARS__

typedef struct {
    int profit;
    int weight;
    int have_restrictions;
} product_t;

typedef struct {
    int r1;
    int r2;
} danger_t;

extern void set_timer();

extern product_t *items;
extern danger_t *dangers;
extern int n_items;
extern int pro_pairs;
extern int ks_cap;
extern int opt_p;
extern int node;
extern int *X;
extern int *opt_x;
extern unsigned global_time;

#endif