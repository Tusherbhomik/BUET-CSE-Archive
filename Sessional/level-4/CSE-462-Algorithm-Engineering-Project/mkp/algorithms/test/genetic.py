"""
Genetic Algorithm for MKP.

Chromosome : binary vector of length n
Fitness    : profit - penalty * sum of constraint violations
             (penalty = max_profit + 1 to strongly discourage infeasibility)
Selection  : tournament (k=3)
Crossover  : single-point (rate=0.8)
Mutation   : bit-flip per gene (rate=1/n)
Repair     : remove items by ascending Toyoda ratio until feasible
Termination: max_generations (default 500) or time limit (default 30 s)
"""

import time
import numpy as np
from ...instance import MKPInstance


def _toyoda_ratio(instance: MKPInstance) -> np.ndarray:
    p = instance.profits.astype(float)
    w = instance.weights.astype(float)
    c = instance.capacities.astype(float)
    safe_c = np.where(c > 0, c, 1.0)
    denom = (w / safe_c[:, None]).sum(axis=0)
    return np.where(denom > 0, p / denom, np.inf)


def _repair(solution: np.ndarray, instance: MKPInstance,
            ratio: np.ndarray) -> np.ndarray:
    """Remove items by ascending ratio until all constraints satisfied."""
    sol = solution.copy()
    # Items currently selected, sorted by ratio ascending (worst first)
    selected = np.where(sol == 1)[0]
    order = selected[np.argsort(ratio[selected])]
    for j in order:
        used = instance.weights @ sol
        if np.all(used <= instance.capacities):
            break
        sol[j] = 0
    return sol


def _fitness(sol: np.ndarray, instance: MKPInstance, penalty: float) -> float:
    profit = float(instance.profits @ sol)
    used = instance.weights @ sol
    violation = float(np.sum(np.maximum(0, used - instance.capacities)))
    return profit - penalty * violation


def _tournament(pop: np.ndarray, fit: np.ndarray,
                rng: np.random.Generator, k: int = 3) -> int:
    idx = rng.integers(0, len(pop), size=k)
    return int(idx[np.argmax(fit[idx])])


def _crossover(p1: np.ndarray, p2: np.ndarray,
               rng: np.random.Generator) -> tuple:
    n = len(p1)
    pt = int(rng.integers(1, n))
    c1 = np.concatenate([p1[:pt], p2[pt:]])
    c2 = np.concatenate([p2[:pt], p1[pt:]])
    return c1, c2


def _mutate(sol: np.ndarray, mut_rate: float,
            rng: np.random.Generator) -> np.ndarray:
    mask = rng.random(len(sol)) < mut_rate
    return np.where(mask, 1 - sol, sol)


def genetic_algorithm(
    instance: MKPInstance,
    pop_size: int = 100,
    max_generations: int = 500,
    cx_rate: float = 0.8,
    time_limit: float = 30.0,
    seed: int = 42,
):
    """
    Returns
    -------
    best_solution    : np.ndarray (n,)
    best_profit      : int
    feasible         : bool
    convergence      : list[int]  best profit at each generation
    """
    n = instance.n
    rng = np.random.default_rng(seed)
    ratio = _toyoda_ratio(instance)
    mut_rate = 1.0 / n
    penalty = float(instance.profits.sum()) + 1.0

    # Initial population: random binary, then repair
    pop = rng.integers(0, 2, size=(pop_size, n)).astype(np.int64)
    pop = np.array([_repair(ind, instance, ratio) for ind in pop])

    fit = np.array([_fitness(ind, instance, penalty) for ind in pop])

    best_idx = int(np.argmax(fit))
    best_sol = pop[best_idx].copy()
    best_profit = int(instance.profits @ best_sol)
    convergence = []

    start = time.perf_counter()

    for gen in range(max_generations):
        if time.perf_counter() - start > time_limit:
            break

        new_pop = []
        while len(new_pop) < pop_size:
            p1 = pop[_tournament(pop, fit, rng)]
            p2 = pop[_tournament(pop, fit, rng)]

            if rng.random() < cx_rate:
                c1, c2 = _crossover(p1, p2, rng)
            else:
                c1, c2 = p1.copy(), p2.copy()

            c1 = _repair(_mutate(c1, mut_rate, rng), instance, ratio)
            c2 = _repair(_mutate(c2, mut_rate, rng), instance, ratio)
            new_pop.extend([c1, c2])

        pop = np.array(new_pop[:pop_size])
        fit = np.array([_fitness(ind, instance, penalty) for ind in pop])

        gen_best_idx = int(np.argmax(fit))
        gen_best_profit = int(instance.profits @ pop[gen_best_idx])
        if gen_best_profit > best_profit:
            best_profit = gen_best_profit
            best_sol = pop[gen_best_idx].copy()

        convergence.append(best_profit)

    feasible = bool(np.all(instance.weights @ best_sol <= instance.capacities))
    return best_sol, best_profit, feasible, convergence
