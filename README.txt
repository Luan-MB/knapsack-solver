Program that solves the Knapsack Problem, with or without restrictions, using a branch and bound algorithm.

* A restriction means that certain products cannot be in the knapsack together.

Input format:

number_of_products(N)   number_of_restrictions(R)  knapsack_capacity(C)

weight_p1   weight_p2   ...   weight_pN

value_p1   value_p2   ...   value_pN

restriction1_pA   restriction1_pB

restriction2_pA   restriction2_pB

...

restrictionR_pa   restrictionR_pb


Output format:


solution (the maximum obtainable profit)

solution_p1   solution_p2   ...   solution_pX  (products that must be in the knapsack for obtaining such solution)