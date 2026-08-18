from dataclasses import dataclass, field
import numpy as np


@dataclass
class MKPInstance:
    n: int                      # number of items
    m: int                      # number of constraints
    profits: np.ndarray         # shape (n,)
    weights: np.ndarray         # shape (m, n)
    capacities: np.ndarray      # shape (m,)
    name: str = ""
    known_optimum: int = -1     # -1 if unknown

    def __repr__(self):
        return (
            f"MKPInstance(name={self.name!r}, n={self.n}, m={self.m}, "
            f"sum_profits={int(self.profits.sum())}, known_opt={self.known_optimum})"
        )
