"""
Generate charts from experiment results CSV (4 algorithms only).

Algorithms:
  toyoda          — toyoda_profit / toyoda_ms
  modified_toyoda         — improved_toyoda_profit / improved_toyoda_ms
  randomized_greedy — randomized_greedy_best / randomized_greedy_ms
  modified_rand_greedy    — improved_randomized_greedy_best / improved_randomized_greedy_ms

Charts produced:
  1. Runtime vs n           (grouped bars, log-scale y)
  2. Profit comparison      (grouped bars per dataset/alpha group)
  3. DynRG / ModRG best-of-N convergence on one instance

Usage:
    python experiments/plots.py [--csv results/full_results.csv]
                                [--out-dir results/figures]
                                [--instance <name>]
"""

import argparse
import csv
import os
import sys
from pathlib import Path
from collections import defaultdict

sys.path.insert(0, str(Path(__file__).parent.parent))

try:
    import matplotlib
    matplotlib.use("Agg")
    import matplotlib.pyplot as plt
    import numpy as np
except ImportError:
    print("matplotlib / numpy not installed. Run: pip install matplotlib numpy")
    sys.exit(1)

# ── Algorithm metadata ────────────────────────────────────────────────────────
ALGO_LABELS = {
    "toyoda": "Toyoda",
    "improved_toyoda": "Modified Toyoda",
    "randomized_greedy":     "Randomized Greedy",
    "improved_randomized_greedy":     "Modified Randomized Greedy",
}
ALGO_COLORS = {
    "toyoda": "#4C72B0",
    "improved_toyoda": "#2196F3",
    "randomized_greedy":     "#DD8452",
    "improved_randomized_greedy":     "#C44E52",
}

# Maps algo key → (profit_col, ms_col)
ALGO_COLS = {
    "toyoda": ("toyoda_profit", "toyoda_ms"),
    "improved_toyoda": ("improved_toyoda_profit", "improved_toyoda_ms"),
    "randomized_greedy":     ("randomized_greedy_best",       "randomized_greedy_ms"),
    "improved_randomized_greedy":     ("improved_randomized_greedy_best",        "improved_randomized_greedy_ms"),
}
ALGOS = list(ALGO_LABELS.keys())


def load_csv(path):
    with open(path) as f:
        return list(csv.DictReader(f))


def _safe_float(v):
    try:
        return float(v)
    except (TypeError, ValueError):
        return None


# ── Chart 1: Runtime vs n ─────────────────────────────────────────────────────
def plot_runtime_vs_n(rows, out_dir):
    """Grouped bar chart: average runtime per n, one group of 4 bars each."""
    data = defaultdict(lambda: defaultdict(list))
    for r in rows:
        n       = r.get("n")
        ds      = r.get("dataset", "")
        alpha   = r.get("alpha", "")
        if not n or not alpha:
            continue
        for algo, (_, ms_col) in ALGO_COLS.items():
            t = _safe_float(r.get(ms_col))
            if t is not None:
                data[n][algo].append(t)

    ns = sorted(data.keys(), key=int)
    if not ns:
        print("[plots] No runtime data, skipping chart 1.")
        return

    x = np.arange(len(ns))
    width = 0.20
    fig, ax = plt.subplots(figsize=(10, 5))

    for i, algo in enumerate(ALGOS):
        means = []
        for n in ns:
            vals = data[n].get(algo, [])
            means.append(np.mean(vals) if vals else 0)
        offset = (i - len(ALGOS) / 2 + 0.5) * width
        ax.bar(x + offset, means, width,
               label=ALGO_LABELS[algo],
               color=ALGO_COLORS[algo],
               edgecolor="white", linewidth=0.4)

    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels([str(n) for n in ns])
    ax.set_xlabel("Number of Items (n)")
    ax.set_ylabel("Runtime (ms, log scale)")
    ax.set_title("Algorithm Runtime vs Problem Size (4 Algorithms)")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.4)

    path = os.path.join(out_dir, "chart1_runtime_vs_n.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[plots] Saved {path}")


# ── Chart 2: Profit comparison ────────────────────────────────────────────────
def plot_profit_comparison(rows, out_dir):
    """Grouped bars: average profit per (n × m × alpha) label, 4 algorithms."""
    data = defaultdict(lambda: defaultdict(list))
    for r in rows:
        n = r.get("n"); m = r.get("m"); alpha = r.get("alpha", "")
        if not alpha:
            continue
        label = f"n={n},m={m}\nα={alpha}" if alpha else f"n={n},m={m}"
        for algo, (p_col, _) in ALGO_COLS.items():
            profit = _safe_float(r.get(p_col))
            if profit is not None:
                data[label][algo].append(profit)

    labels = sorted(data.keys())
    if not labels:
        print("[plots] No profit data, skipping chart 2.")
        return

    x = np.arange(len(labels))
    width = 0.20
    fig, ax = plt.subplots(figsize=(max(8, len(labels) * 2), 5))

    for i, algo in enumerate(ALGOS):
        means = []
        for lbl in labels:
            vals = data[lbl].get(algo, [])
            means.append(np.mean(vals) if vals else 0)
        offset = (i - len(ALGOS) / 2 + 0.5) * width
        ax.bar(x + offset, means, width,
               label=ALGO_LABELS[algo], color=ALGO_COLORS[algo],
               edgecolor="white", linewidth=0.4)

    ax.set_xticks(x)
    ax.set_xticklabels(labels, fontsize=8)
    ax.set_xlabel("Dataset")
    ax.set_ylabel("Average Profit")
    ax.set_title("Profit Comparison by Algorithm and Dataset (4 Algorithms)")
    ax.legend()
    ax.grid(axis="y", linestyle="--", alpha=0.4)

    path = os.path.join(out_dir, "chart2_profit_comparison.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[plots] Saved {path}")


# ── Chart 3: RG convergence (best-of-N) ──────────────────────────────────────
def plot_rg_convergence(instance_name: str, out_dir: str):
    """Re-run DynRG and ModRG on one instance and plot best-of-N curves."""
    from mkp.parser import parse_instance
    from mkp.algorithms.randomized_greedy import randomized_greedy as _drg
    from mkp.algorithms.improved_randomized_greedy import modified_randomized_greedy as _mrg
    from mkp.algorithms.toyoda import toyoda as _dtoy

    base = Path(__file__).parent.parent
    matches = list(base.rglob(f"*{instance_name}*.dat"))
    if not matches:
        print(f"[plots] Instance file not found for '{instance_name}', skipping chart 3.")
        return
    inst = parse_instance(str(matches[0]))
    print(f"[plots] RG convergence on {inst.name} (n={inst.n}, m={inst.m})")

    _, dyn_toy_p, _ = _dtoy(inst)

    n_iter = 50
    _, _, _, drg_all = _drg(inst, k=5, n_iter=n_iter, seed=42)
    _, _, _, mrg_all = _mrg(inst, k=5, alpha=0.3, n_iter=n_iter, seed=42)

    fig, ax = plt.subplots(figsize=(8, 4))
    for profits, label, color in [
        (drg_all, "Randomized Greedy",          ALGO_COLORS["randomized_greedy"]),
        (mrg_all, "Modified Randomized Greedy", ALGO_COLORS["improved_randomized_greedy"]),
    ]:
        best_so_far = [max(profits[:i+1]) for i in range(len(profits))]
        ax.plot(range(1, len(best_so_far)+1), best_so_far,
                color=color, linewidth=1.5, label=label)

    ax.axhline(dyn_toy_p, color=ALGO_COLORS["toyoda"], linestyle="--",
               linewidth=1.2, label=f"Toyoda ({dyn_toy_p:,})")
    ax.set_xlabel("Restarts")
    ax.set_ylabel("Best Profit")
    ax.set_title(f"RG Convergence — {inst.name} (n={inst.n}, m={inst.m})")
    ax.legend()
    ax.grid(linestyle="--", alpha=0.4)

    path = os.path.join(out_dir, "chart3_rg_convergence.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"[plots] Saved {path}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--csv", default="results/full_results.csv")
    parser.add_argument("--out-dir", default="results/figures")
    parser.add_argument("--instance", default="OR5x100-0.25_1",
                        help="Instance name for RG convergence plot")
    args = parser.parse_args()

    os.makedirs(args.out_dir, exist_ok=True)

    if os.path.exists(args.csv):
        rows = load_csv(args.csv)
        plot_runtime_vs_n(rows, args.out_dir)
        plot_profit_comparison(rows, args.out_dir)
    else:
        print(f"[plots] CSV not found: {args.csv}. Skipping charts 1 & 2.")

    plot_rg_convergence(args.instance, args.out_dir)


if __name__ == "__main__":
    main()