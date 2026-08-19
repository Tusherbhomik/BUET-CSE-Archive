import numpy as np
from .instance import MKPInstance


def is_feasible(instance: MKPInstance, solution: np.ndarray) -> bool:
    """Return True if solution satisfies all capacity constraints."""
    used = instance.weights @ solution          # shape (m,)
    return bool(np.all(used <= instance.capacities))


def evaluate(instance: MKPInstance, solution: np.ndarray):
    """Return (profit, feasible) for a binary solution vector."""
    profit = int(instance.profits @ solution)
    feasible = is_feasible(instance, solution)
    return profit, feasible


def optimality_gap(profit: int, known_opt: int) -> float:
    """Return % gap from known optimum. Returns NaN if optimum unknown."""
    if known_opt <= 0:
        return float("nan")
    return 100.0 * (known_opt - profit) / known_opt
