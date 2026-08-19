"""
Randomized Greedy algorithm for MKP.

Toyoda ratio: recomputed at every step using current remaining
capacities, consistent with Feo & Resende (1995) GRASP construction phase.
Multi-restart: runs n_iter independent trials and returns the best.

Complexity per run: O(n^2 * m);  total O(n_iter * n^2 * m)
"""

import numpy as np
from ..instance import MKPInstance


def _single_run(
    instance: MKPInstance,
    k: int,
    rng: np.random.Generator,
) -> np.ndarray:
    """One randomized greedy pass with Toyoda ratio recomputation."""
    w = instance.weights.astype(float)          # shape (m, n)
    p = instance.profits.astype(float)          # shape (n,)
    remaining = instance.capacities.astype(float)  # shape (m,)
    solution = np.zeros(instance.n, dtype=np.int64)
    unselected = list(range(instance.n))

    while unselected:
        # --- Step 1: Dynamically recompute Toyoda ratio ---
        # Use current remaining capacities (R_i), not original C_i
        safe_r = np.where(remaining > 0, remaining, 1.0)
        denom = (w[:, unselected] / safe_r[:, None]).sum(axis=0)
        ratios = np.where(denom > 0, p[unselected] / denom, np.inf)

        # --- Step 2: Build RCL - top-k feasible by current ratio ---
        order = np.argsort(-ratios)  # descending ratio
        pool = []
        for idx in order:
            if len(pool) == k:
                break
            j = unselected[idx]
            if np.all(w[:, j] <= remaining):
                pool.append(j)

        if not pool:
            break  # no feasible item remains

        # --- Step 3: Select uniformly at random from RCL ---
        chosen = int(rng.choice(pool))
        solution[chosen] = 1
        remaining -= w[:, chosen]
        unselected.remove(chosen)

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
    k      : restricted candidate list size (RCL)
    n_iter : number of independent restarts
    seed   : random seed for reproducibility

    Returns
    -------
    best_solution : np.ndarray (n,)
    best_profit   : int
    feasible      : bool
    all_profits   : list[int]  (one per iteration, for analysis)
    """
    rng = np.random.default_rng(seed)
    best_solution = None
    best_profit = -1
    all_profits = []

    for _ in range(n_iter):
        sol = _single_run(instance, k, rng)
        profit = int(instance.profits @ sol)
        all_profits.append(profit)
        if profit > best_profit:
            best_profit = profit
            best_solution = sol.copy()

    feasible = bool(
        np.all(instance.weights @ best_solution <= instance.capacities)
    )
    return best_solution, best_profit, feasible, all_profits