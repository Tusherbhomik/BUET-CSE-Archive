"""
Randomized Greedy algorithm for MKP.

Uses Toyoda's ratio ordering but at each step picks uniformly at random
from the top-k candidates (that are still feasible to add).
Multi-restart: runs n_iter independent trials and returns the best.

Complexity per run: O(n*m);  total O(n_iter * n * m)
"""

import numpy as np
from ...instance import MKPInstance


def _single_run(
    instance: MKPInstance,
    ratio_order: np.ndarray,
    k: int,
    rng: np.random.Generator,
):
    """One randomized greedy pass."""
    w = instance.weights
    c = instance.capacities.astype(float)
    remaining = c.copy()
    solution = np.zeros(instance.n, dtype=np.int64)

    candidates = list(ratio_order)  # ordered by ratio desc
    while candidates:
        # collect up to k feasible candidates from the front
        pool = []
        for j in candidates:
            if len(pool) == k:
                break
            if np.all(w[:, j] <= remaining):
                pool.append(j)
        if not pool:
            break
        chosen = rng.choice(pool)
        solution[chosen] = 1
        remaining -= w[:, chosen]
        candidates.remove(chosen)

    return solution


def randomized_greedy(
    instance: MKPInstance,
    k: int = 5,
    n_iter: int = 50,
    seed: int = 42,
):
    """
    Parameters
    ----------
    k      : restricted candidate list size
    n_iter : number of independent restarts
    seed   : random seed for reproducibility

    Returns
    -------
    best_solution : np.ndarray (n,)
    best_profit   : int
    feasible      : bool
    all_profits   : list[int]  (one per iteration, for analysis)
    """
    n, m = instance.n, instance.m
    p = instance.profits.astype(float)
    w = instance.weights.astype(float)
    c = instance.capacities.astype(float)

    safe_c = np.where(c > 0, c, 1.0)
    denom = (w / safe_c[:, None]).sum(axis=0)
    ratio = np.where(denom > 0, p / denom, np.inf)
    ratio_order = np.argsort(-ratio)

    rng = np.random.default_rng(seed)
    best_solution = None
    best_profit = -1
    all_profits = []

    for _ in range(n_iter):
        sol = _single_run(instance, ratio_order, k, rng)
        profit = int(instance.profits @ sol)
        all_profits.append(profit)
        if profit > best_profit:
            best_profit = profit
            best_solution = sol.copy()

    feasible = bool(np.all(instance.weights @ best_solution <= instance.capacities))
    return best_solution, best_profit, feasible, all_profits
