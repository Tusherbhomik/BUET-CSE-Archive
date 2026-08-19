import numpy as np
from ..instance import MKPInstance


def toyoda(instance: MKPInstance):
    """
    Toyoda heuristic for MKP (recomputes ratios each step)

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

    solution = np.zeros(n, dtype=np.int64)
    remaining = c.copy()
    selected = np.zeros(n, dtype=bool)

    while True:
        best_j = -1
        best_ratio = -np.inf

        # Avoid division by zero for capacities
        safe_remaining = np.where(remaining > 0, remaining, 1.0)

        for j in range(n):
            if selected[j]:
                continue

            # Compute denominator using CURRENT remaining capacities
            denom = np.sum(w[:, j] / safe_remaining)

            # Handle weightless items
            ratio = p[j] / denom if denom > 0 else np.inf

            # Check feasibility
            if np.all(w[:, j] <= remaining):
                if ratio > best_ratio:
                    best_ratio = ratio
                    best_j = j

        # Stop if no feasible item found
        if best_j == -1:
            break

        # Add best item
        solution[best_j] = 1
        selected[best_j] = True
        remaining -= w[:, best_j]

    profit = int(instance.profits @ solution)
    feasible = bool(np.all(instance.weights @ solution <= instance.capacities))

    return solution, profit, feasible