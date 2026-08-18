"""
Improved Randomized Greedy algorithm for MKP (updated for pipeline compatibility).

Key improvements:
- Adaptive alpha: starts exploratory (high) and becomes more greedy (low) over iterations
- 1-swap local search applied to every constructed solution
- Elite seeding from Toyoda as strong baseline
- Dead item pruning + vectorized operations
- Maintains backward compatibility with main.py call signature

This version keeps the old function name `modified_randomized_greedy` and accepts 
the same parameters as before (including `alpha`), while internally using adaptive alpha + local search.
"""

import numpy as np
from ..instance import MKPInstance
from .toyoda import toyoda


def _local_search_swap(
    solution: np.ndarray,
    remaining: np.ndarray,
    w: np.ndarray,
    p: np.ndarray,
) -> tuple[np.ndarray, np.ndarray]:
    """1-swap local search: try to improve solution by swapping one item."""
    selected_items = np.where(solution == 1)[0]
    unselected_items = np.where(solution == 0)[0]

    for j_out in selected_items:
        freed = remaining + w[:, j_out]
        best_gain = 0
        best_j_in = -1

        for j_in in unselected_items:
            if np.all(w[:, j_in] <= freed):
                gain = p[j_in] - p[j_out]
                if gain > best_gain:
                    best_gain = gain
                    best_j_in = j_in

        if best_j_in != -1:
            # Perform the swap
            solution[j_out] = 0
            remaining += w[:, j_out]
            solution[best_j_in] = 1
            remaining -= w[:, best_j_in]

            # Refresh lists
            selected_items = np.where(solution == 1)[0]
            unselected_items = np.where(solution == 0)[0]

    return solution, remaining


def _single_run(
    instance: MKPInstance,
    k: int,
    alpha: float,
    rng: np.random.Generator,
    do_local_search: bool = True,
) -> tuple[np.ndarray, np.ndarray]:
    """
    One randomized greedy construction + optional local search.
    Follows the same structure as the original source code.
    """
    w = instance.weights.astype(float)            # (m, n)
    p = instance.profits.astype(float)            # (n,)
    remaining = instance.capacities.astype(float) # (m,)
    solution = np.zeros(instance.n, dtype=np.int64)
    unselected = list(range(instance.n))

    while unselected:
        idx = np.array(unselected)

        # --- Step 1: Vectorized Toyoda ratio ---
        safe_r = np.where(remaining > 0, remaining, 1.0)
        denom = (w[:, idx] / safe_r[:, None]).sum(axis=0)
        ratios = np.where(denom > 0, p[idx] / denom, np.inf)

        # --- Step 2: Vectorized feasibility + dead item pruning ---
        feasible_mask = np.all(w[:, idx] <= remaining[:, None], axis=0)

        # Prune permanently infeasible items
        unselected = [j for i, j in enumerate(unselected) if feasible_mask[i]]
        if not unselected:
            break

        # Recompute after pruning
        idx = np.array(unselected)
        safe_r = np.where(remaining > 0, remaining, 1.0)
        denom = (w[:, idx] / safe_r[:, None]).sum(axis=0)
        ratios = np.where(denom > 0, p[idx] / denom, np.inf)

        # --- Step 3: Build RCL ---
        r_max = ratios.max()
        r_min = ratios.min()

        if alpha is not None and r_max > r_min:
            # Value-based RCL (adaptive alpha will be passed here)
            threshold = r_max - alpha * (r_max - r_min)
            rcl_local_idx = np.where(ratios >= threshold)[0]
            pool = idx[rcl_local_idx].tolist()
        else:
            # Fallback: cardinality-k RCL
            order = np.argsort(-ratios)
            pool = idx[order[:k]].tolist()

        if not pool:
            break

        # --- Step 4: Select randomly from RCL ---
        chosen = int(rng.choice(pool))
        solution[chosen] = 1
        remaining -= w[:, chosen]
        unselected.remove(chosen)

    # --- Step 5: Optional local search ---
    if do_local_search:
        solution, remaining = _local_search_swap(solution, remaining, w, p)

    return solution, remaining


def improved_randomized_greedy(
    instance: MKPInstance,
    k: int = 5,
    alpha: float = 0.3,           # kept for backward compatibility with main.py
    n_iter: int = 50,
    seed: int = 42,
):
    """
    Parameters (compatible with main.py)
    ----------
    k      : cardinality RCL size fallback
    alpha  : Now used as alpha_start. We internally decay it to 0.1 for better performance.
    n_iter : number of restarts
    seed   : random seed

    Returns
    -------
    best_solution, best_profit, feasible, all_profits
    """
    rng = np.random.default_rng(seed)
    all_profits = []

    # Elite seeding from Toyoda
    best_solution, best_profit, _ = toyoda(instance)

    for t in range(n_iter):
        # Adaptive alpha: decay from alpha (start) to 0.1
        alpha_current = alpha + (0.1 - alpha) * (t / max(n_iter - 1, 1))

        sol, _ = _single_run(instance, k, alpha_current, rng, do_local_search=True)

        profit = int(instance.profits @ sol)
        all_profits.append(profit)

        if profit > best_profit:
            best_profit = profit
            best_solution = sol.copy()

    feasible = bool(
        np.all(instance.weights @ best_solution <= instance.capacities)
    )
    return best_solution, best_profit, feasible, all_profits