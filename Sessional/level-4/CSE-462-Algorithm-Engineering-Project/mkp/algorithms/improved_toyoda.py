import numpy as np
from ..instance import MKPInstance


def improved_toyoda(instance: MKPInstance, max_swap_rounds: int = 3):
    n, m = instance.n, instance.m
    p = instance.profits.astype(float)
    w = instance.weights.astype(float)   # shape (m, n)
    c = instance.capacities.astype(float)

    solution = np.zeros(n, dtype=np.int64)
    remaining = c.copy()
    selected = np.zeros(n, dtype=bool)

    # -------------------------------------------------------
    # Phase 1: Standard Toyoda (identical to original)
    # -------------------------------------------------------
    while True:
        best_j = -1
        best_ratio = -np.inf
        safe_remaining = np.where(remaining > 0, remaining, 1.0)

        for j in range(n):
            if selected[j]:
                continue
            if np.all(w[:, j] <= remaining):
                denom = np.sum(w[:, j] / safe_remaining)
                ratio = p[j] / denom if denom > 0 else np.inf
                if ratio > best_ratio:
                    best_ratio = ratio
                    best_j = j

        if best_j == -1:
            break

        solution[best_j] = 1
        selected[best_j] = True
        remaining -= w[:, best_j]

    # -------------------------------------------------------
    # Phase 2: Local Search -- 1-swap improvement
    # For each selected item j_out, try replacing it with
    # the best unselected item j_in that:
    #   (a) fits after j_out is removed
    #   (b) has strictly higher profit than j_out
    # -------------------------------------------------------
    improved = True
    rounds = 0

    while improved and rounds < max_swap_rounds:
        improved = False
        rounds += 1

        selected_items = np.where(selected)[0]
        unselected_items = np.where(~selected)[0]

        for j_out in selected_items:
            # Tentatively free capacity by removing j_out
            freed_remaining = remaining + w[:, j_out]

            best_gain = 0          # must beat zero (strict improvement)
            best_j_in = -1

            for j_in in unselected_items:
                # Check if j_in fits after removing j_out
                if not np.all(w[:, j_in] <= freed_remaining):
                    continue

                gain = p[j_in] - p[j_out]
                if gain > best_gain:
                    best_gain = gain
                    best_j_in = j_in

            if best_j_in != -1:
                # Perform the swap
                solution[j_out] = 0
                selected[j_out] = False
                remaining += w[:, j_out]      # free j_out

                solution[best_j_in] = 1
                selected[best_j_in] = True
                remaining -= w[:, best_j_in]  # consume j_in

                improved = True  # signal another round may help


    # -------------------------------------------------------
    # Phase 3: Fill-Up after swaps (now actually useful)
    # Swaps may free residual capacity for new items
    # -------------------------------------------------------
    unselected_indices = np.where(~selected)[0]
    if len(unselected_indices) > 0:
        sum_weights = w[:, unselected_indices].sum(axis=0)
        densities = p[unselected_indices] / np.where(
            sum_weights > 0, sum_weights, 1.0
        )
        fill_order = unselected_indices[np.argsort(-densities)]
        for j in fill_order:
            if np.all(w[:, j] <= remaining):
                solution[j] = 1
                selected[j] = True
                remaining -= w[:, j]

    profit = int(instance.profits @ solution)
    feasible = bool(
        np.all(instance.weights @ solution <= instance.capacities)
    )
    return solution, profit, feasible