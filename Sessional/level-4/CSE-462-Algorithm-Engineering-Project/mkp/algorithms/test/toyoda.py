"""
Toyoda's greedy algorithm for the Multidimensional Knapsack Problem.

Ratio: r_j = p_j / sum_i( w_{ij} / C_i )
Items are sorted in descending order of ratio and added greedily if feasible.

Complexity: O(n*m log n)
"""

import numpy as np
from ...instance import MKPInstance


def toyoda(instance: MKPInstance):
    """
    Returns
    -------
    solution : np.ndarray of shape (n,), dtype int (0/1)
    profit   : int
    feasible : bool
    """
    n, m = instance.n, instance.m
    p = instance.profits.astype(float)
    w = instance.weights.astype(float)
    c = instance.capacities.astype(float)

    # Capacity-normalised weight denominator per item
    # ratio_j = p_j / sum_i(w_ij / C_i)
    # Avoid div-by-zero: if C_i == 0 treat constraint as inactive
    safe_c = np.where(c > 0, c, 1.0)
    denom = (w / safe_c[:, None]).sum(axis=0)      # shape (n,)
    # If denom == 0 (weightless item), assign infinite ratio
    ratio = np.where(denom > 0, p / denom, np.inf)

    order = np.argsort(-ratio)   # descending

    solution = np.zeros(n, dtype=np.int64)
    remaining = c.copy()

    for j in order:
        if np.all(w[:, j] <= remaining):
            solution[j] = 1
            remaining -= w[:, j]

    profit = int(instance.profits @ solution)
    feasible = bool(np.all(instance.weights @ solution <= instance.capacities))
    return solution, profit, feasible
