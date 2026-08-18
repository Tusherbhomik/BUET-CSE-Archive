#!/usr/bin/env python3
"""
Full Experiment Runner - Complete Pipeline
==========================================

This script runs the complete experimental pipeline:
1. Runs all 4 algorithms (Toyoda, Improved Toyoda, Randomized Greedy, Improved RG) 
   on all selected datasets
2. Generates comprehensive results CSV
3. Produces all visualization charts and figures

Algorithms:
  - Toyoda (Dynamic)         — Basic greedy with dynamic ratio computation
  - Improved Toyoda          — Enhanced with fill-up pass
  - Randomized Greedy (RG)   — GRASP-style with k-RCL and multi-restart
  - Improved RG              — Alpha-threshold RCL + elite seed + pruning

Datasets (default):
  - OR5x100, OR10x250, OR30x500 (default subset)
  
All available datasets (use --all flag):
  - OR5x100, OR10x100, OR30x100 (n=100)
  - OR5x250, OR10x250, OR30x250 (n=250) 
  - OR5x500, OR10x500, OR30x500 (n=500)
  - gk (Glover-Kochenberger instances)
  - sac94 (SAC-94 benchmark instances)

Outputs:
  - results/full_results.csv              — Complete experimental data
  - results/figures/chart1_runtime_vs_n.png     — Runtime comparison
  - results/figures/chart2_profit_comparison.png — Profit comparison
  - results/figures/chart3_rg_convergence.png   — RG convergence analysis
  - results/figures/chart4_optimality_gaps.png  — Optimality gap comparison
  - results/figures/chart5_improvement_analysis.png — Improvement percentages

Usage:
  python run_full_experiment.py              # Default subset (OR5x100, OR10x250, OR30x500)
  python run_full_experiment.py --all        # ALL datasets (300+ instances!)
  python run_full_experiment.py --quick      # Quick test (2 instances/dataset or 10 total)
  python run_full_experiment.py --datasets OR5x100 OR10x250  # Selected datasets only
  python run_full_experiment.py --rg-iter 50  # More RG iterations
"""

import argparse
import csv
import math
import os
import sys
import time
from pathlib import Path
from collections import defaultdict

# Add parent to path
sys.path.insert(0, str(Path(__file__).parent))

from mkp.parser import load_dataset_dir, parse_instance
from mkp.metrics import optimality_gap
from mkp.algorithms.toyoda import toyoda
from mkp.algorithms.improved_toyoda import improved_toyoda
from mkp.algorithms.randomized_greedy import randomized_greedy
from mkp.algorithms.improved_randomized_greedy import improved_randomized_greedy

# ── Configuration ─────────────────────────────────────────────────────────────

BASE = Path(__file__).parent / "All-MKP-Instances"

# All available datasets
ALL_DATASETS = {
    # Chu-Beasley OR-Library instances (9 datasets)
    "OR5x100":   BASE / "chubeas" / "OR5x100",
    "OR10x100":  BASE / "chubeas" / "OR10x100",
    "OR30x100":  BASE / "chubeas" / "OR30x100",
    "OR5x250":   BASE / "chubeas" / "OR5x250",
    "OR10x250":  BASE / "chubeas" / "OR10x250",
    "OR30x250":  BASE / "chubeas" / "OR30x250",
    "OR5x500":   BASE / "chubeas" / "OR5x500",
    "OR10x500":  BASE / "chubeas" / "OR10x500",
    "OR30x500":  BASE / "chubeas" / "OR30x500",
    # Glover-Kochenberger instances
    "gk":        BASE / "gk",
    # SAC-94 benchmark instances
    "sac94":     BASE / "sac94",
}

# Default datasets for experiments (quick subset)
DEFAULT_DATASETS = ["OR5x100", "OR10x250", "OR30x500"]

FIELDNAMES = [
    # Instance metadata
    "group", "dataset", "instance", "n", "m", "alpha", "known_opt",
    
    # Toyoda
    "toyoda_profit", "toyoda_ms",
    
    # Improved Toyoda
    "improved_toyoda_profit", "improved_toyoda_ms",
    
    # Randomized Greedy
    "randomized_greedy_best", "randomized_greedy_avg", "randomized_greedy_std", "randomized_greedy_ms",
    
    # Improved Randomized Greedy
    "improved_randomized_greedy_best", "improved_randomized_greedy_avg", "improved_randomized_greedy_std", "improved_randomized_greedy_ms",
    
    # Optimality gaps
    "toyoda_gap", "improved_toyoda_gap", "randomized_greedy_gap", "improved_randomized_greedy_gap",
    
    # Improvement percentages
    "improved_toyoda_vs_toyoda_pct",
    "improved_randomized_greedy_vs_randomized_greedy_pct",
    "randomized_greedy_vs_toyoda_pct",
    "improved_randomized_greedy_vs_improved_toyoda_pct",
]

# ── Helper Functions ──────────────────────────────────────────────────────────

def _rg_stats(profits):
    """Calculate average and standard deviation of RG profits."""
    avg = sum(profits) / len(profits)
    std = math.sqrt(sum((x - avg) ** 2 for x in profits) / len(profits))
    return round(avg, 2), round(std, 2)


def _gap(profit, opt):
    """Calculate optimality gap as percentage."""
    return round(optimality_gap(profit, opt), 3) if opt and opt > 0 else ""


def _pct_gain(new_p, base_p):
    """Calculate percentage gain."""
    return round(100 * (new_p - base_p) / base_p, 3) if base_p else ""


def _alpha_from_name(name: str) -> str:
    """Extract alpha value from instance filename (e.g., OR5x100-0.25_1)."""
    for part in name.split("-"):
        for sub in part.split("_"):
            try:
                v = float(sub)
                if 0 < v < 1:
                    return sub
            except ValueError:
                pass
    return ""


# ── Algorithm Runners ─────────────────────────────────────────────────────────

def run_toyoda(inst):
    """Run basic Toyoda algorithm."""
    t0 = time.perf_counter()
    _, profit, _ = toyoda(inst)
    ms = (time.perf_counter() - t0) * 1000
    return profit, round(ms, 3)


def run_improved_toyoda(inst):
    """Run Improved Toyoda algorithm."""
    t0 = time.perf_counter()
    _, profit, _ = improved_toyoda(inst)
    ms = (time.perf_counter() - t0) * 1000
    return profit, round(ms, 3)


def run_rg(inst, n_iter=20, k=5, seed=42):
    """Run Randomized Greedy algorithm."""
    t0 = time.perf_counter()
    _, profit, _, all_profits = randomized_greedy(
        inst, k=k, n_iter=n_iter, seed=seed)
    ms = (time.perf_counter() - t0) * 1000
    avg, std = _rg_stats(all_profits)
    return profit, avg, std, round(ms, 3)


def run_improved_rg(inst, n_iter=20, k=5, alpha=0.3, seed=42):
    """Run Improved Randomized Greedy algorithm."""
    t0 = time.perf_counter()
    _, profit, _, all_profits = improved_randomized_greedy(
        inst, k=k, alpha=alpha, n_iter=n_iter, seed=seed)
    ms = (time.perf_counter() - t0) * 1000
    avg, std = _rg_stats(all_profits)
    return profit, avg, std, round(ms, 3)


def _run_algorithms_on_instance(inst, row, quick, rg_iter, verbose=False):
    """
    Run all 4 algorithms on a single instance and update the row dict.
    
    Args:
        inst: MKPInstance
        row: Dict to update with results
        quick: Boolean for quick mode
        rg_iter: Number of RG iterations
        verbose: Print detailed output
    """
    opt = row.get("known_opt", "") or 0
    if opt == "":
        opt = 0
    
    # Run Toyoda
    try:
        toy_p, toy_ms = run_toyoda(inst)
        row.update(toyoda_profit=toy_p, toyoda_ms=toy_ms)
        if verbose:
            print(f"        Toyoda:                  {toy_p:>8} ({toy_ms:>7.2f} ms)")
        else:
            print(f"T={toy_p}", end="  ", flush=True)
    except Exception as e:
        if verbose:
            print(f"        Toyoda:                  ERROR - {e}")
        row.update(toyoda_profit=0, toyoda_ms="")
        toy_p = 0
    
    # Run Improved Toyoda
    try:
        imp_toy_p, imp_toy_ms = run_improved_toyoda(inst)
        row.update(improved_toyoda_profit=imp_toy_p, improved_toyoda_ms=imp_toy_ms)
        if verbose:
            print(f"        Improved Toyoda:         {imp_toy_p:>8} ({imp_toy_ms:>7.2f} ms)")
        else:
            print(f"MT={imp_toy_p}", end="  ", flush=True)
    except Exception as e:
        if verbose:
            print(f"        Improved Toyoda:         ERROR - {e}")
        row.update(improved_toyoda_profit=0, improved_toyoda_ms="")
        imp_toy_p = 0
    
    # Run Randomized Greedy
    ni = 5 if quick else rg_iter
    try:
        rg_p, rg_avg, rg_std, rg_ms = run_rg(inst, n_iter=ni)
        row.update(randomized_greedy_best=rg_p, randomized_greedy_avg=rg_avg, 
                   randomized_greedy_std=rg_std, randomized_greedy_ms=rg_ms)
        if verbose:
            print(f"        Randomized Greedy:       {rg_p:>8} (avg={rg_avg:.1f}, {rg_ms:>7.2f} ms)")
        else:
            print(f"RG={rg_p}", end="  ", flush=True)
    except Exception as e:
        if verbose:
            print(f"        Randomized Greedy:       ERROR - {e}")
        row.update(randomized_greedy_best=0, randomized_greedy_avg="", randomized_greedy_std="", randomized_greedy_ms="")
        rg_p = 0
    
    # Run Improved Randomized Greedy
    try:
        imp_rg_p, imp_rg_avg, imp_rg_std, imp_rg_ms = run_improved_rg(inst, n_iter=ni)
        row.update(improved_randomized_greedy_best=imp_rg_p, improved_randomized_greedy_avg=imp_rg_avg,
                   improved_randomized_greedy_std=imp_rg_std, improved_randomized_greedy_ms=imp_rg_ms)
        if verbose:
            print(f"        Improved Randomized Greedy: {imp_rg_p:>8} (avg={imp_rg_avg:.1f}, {imp_rg_ms:>7.2f} ms)")
        else:
            print(f"MRG={imp_rg_p}", flush=True)
    except Exception as e:
        if verbose:
            print(f"        Improved Randomized Greedy: ERROR - {e}")
        row.update(improved_randomized_greedy_best=0, improved_randomized_greedy_avg="", improved_randomized_greedy_std="", improved_randomized_greedy_ms="")
        imp_rg_p = 0
    
    # Calculate optimality gaps
    row["toyoda_gap"] = _gap(toy_p, opt)
    row["improved_toyoda_gap"] = _gap(imp_toy_p, opt)
    row["randomized_greedy_gap"] = _gap(rg_p, opt)
    row["improved_randomized_greedy_gap"] = _gap(imp_rg_p, opt)
    
    # Calculate improvement percentages
    row["improved_toyoda_vs_toyoda_pct"] = _pct_gain(imp_toy_p, toy_p)
    row["improved_randomized_greedy_vs_randomized_greedy_pct"] = _pct_gain(imp_rg_p, rg_p)
    row["randomized_greedy_vs_toyoda_pct"] = _pct_gain(rg_p, toy_p)
    row["improved_randomized_greedy_vs_improved_toyoda_pct"] = _pct_gain(imp_rg_p, imp_toy_p)


# ── Main Experiment Runner ────────────────────────────────────────────────────

def run_experiment(datasets, out_path, quick=False, rg_iter=20, use_all_datasets=False):
    """
    Run all 4 algorithms on selected datasets.
    
    Args:
        datasets: Dict of dataset_name -> dataset_path or None for all datasets
        out_path: Output CSV path
        quick: If True, run only 2 instances per dataset
        rg_iter: Number of iterations for RG algorithms
        use_all_datasets: If True, run on ALL datasets (chubeas, gk, sac94)
    
    Returns:
        List of result rows
    """
    rows = []
    
    # If use_all_datasets is True, collect all instances like full_runner.py
    if use_all_datasets:
        tasks = []
        
        # Chubeas instances (skip the combined summary file in each subdir)
        chubeas_dir = BASE / "chubeas"
        if chubeas_dir.exists():
            for ds_dir in sorted(chubeas_dir.iterdir()):
                if ds_dir.is_dir():
                    for f in sorted(ds_dir.glob("*.dat")):
                        if f.stem != ds_dir.name:   # skip OR10x250/OR10x250.dat
                            tasks.append(("chubeas", ds_dir.name, str(f)))

        # GK instances
        gk_dir = BASE / "gk"
        if gk_dir.exists():
            for f in sorted(gk_dir.glob("*.dat")):
                tasks.append(("gk", "gk", str(f)))

        # SAC94 instances
        sac94_dir = BASE / "sac94"
        if sac94_dir.exists():
            for sub in sorted(sac94_dir.iterdir()):
                if sub.is_dir():
                    for f in sorted(sub.glob("*.dat")):
                        tasks.append(("sac94", sub.name, str(f)))
        
        if quick:
            tasks = tasks[:10]  # Just 10 instances for quick mode
        
        total = len(tasks)
        print("\n" + "="*80)
        print(f"  RUNNING FULL EXPERIMENT - ALL DATASETS")
        print(f"  {'Quick mode (10 instances)' if quick else 'Full mode (all instances)'}")
        print(f"  RG iterations: {5 if quick else rg_iter}")
        print(f"  Total instances: {total}")
        print(f"  Datasets: chubeas (9), gk, sac94")
        print("="*80 + "\n")
        
        for idx, (group, dataset, fpath) in enumerate(tasks, 1):
            try:
                inst = parse_instance(fpath)
            except Exception as e:
                print(f"[{idx}/{total}] SKIP {fpath}: {e}")
                continue
            
            name = Path(fpath).stem
            alpha = _alpha_from_name(name)
            opt = inst.known_optimum
            
            print(f"[{idx:3d}/{total}] {group}/{dataset}/{name} "
                  f"(n={inst.n}, m={inst.m})", end="  ", flush=True)
            
            row = dict(
                group=group,
                dataset=dataset,
                instance=name,
                n=inst.n,
                m=inst.m,
                alpha=alpha,
                known_opt=opt if opt > 0 else "",
            )
            
            # Run algorithms and collect results
            _run_algorithms_on_instance(inst, row, quick, rg_iter)
            rows.append(row)
            
    else:
        # Original mode - run on selected datasets
        total_instances = sum(
            len(load_dataset_dir(str(ds_dir))[:2] if quick else load_dataset_dir(str(ds_dir)))
            for ds_dir in datasets.values() if ds_dir.exists()
        )
        
        print("\n" + "="*80)
        print(f"  RUNNING EXPERIMENT")
        print(f"  {'Quick mode (2 instances/dataset)' if quick else 'Full mode (all instances)'}")
        print(f"  RG iterations: {5 if quick else rg_iter}")
        print(f"  Estimated instances: {total_instances}")
        print(f"  Datasets: {', '.join(datasets.keys())}")
        print("="*80 + "\n")
        
        for ds_name, ds_dir in datasets.items():
            if not ds_dir.exists():
                print(f"[!] Dataset dir not found: {ds_dir}, skipping.")
                continue
                
            instances = load_dataset_dir(str(ds_dir))
            if quick:
                instances = instances[:2]
            
            print(f"\n{'='*60}")
            print(f"  Dataset: {ds_name} ({len(instances)} instances)")
            print(f"{'='*60}")
            
            for idx, inst in enumerate(instances, 1):
                alpha = _alpha_from_name(inst.name)
                opt = inst.known_optimum
                
                print(f"\n[{idx}/{len(instances)}] {inst.name}")
                print(f"        n={inst.n}, m={inst.m}, α={alpha}, opt={opt if opt > 0 else 'unknown'}")
                
                # Determine group (chubeas, gk, or sac94)
                group = "chubeas" if "OR" in ds_name else ds_name
                
                row = dict(
                    group=group,
                    dataset=ds_name,
                    instance=inst.name,
                    n=inst.n,
                    m=inst.m,
                    alpha=alpha,
                    known_opt=opt if opt > 0 else "",
                )
                
                # Run algorithms and collect results
                _run_algorithms_on_instance(inst, row, quick, rg_iter, verbose=True)
                rows.append(row)
    
    # Save results to CSV
    os.makedirs(Path(out_path).parent, exist_ok=True)
    with open(out_path, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=FIELDNAMES)
        writer.writeheader()
        writer.writerows(rows)
    
    print(f"\n{'='*80}")
    print(f"  EXPERIMENT COMPLETE")
    print(f"  Total instances: {len(rows)}")
    print(f"  Results saved to: {out_path}")
    print(f"{'='*80}\n")
    
    return rows


# ── Figure Generation ─────────────────────────────────────────────────────────

def generate_figures(csv_path, out_dir, instance_name="OR5x100-0.25_1"):
    """
    Generate all visualization charts from results CSV.
    
    Args:
        csv_path: Path to results CSV
        out_dir: Output directory for figures
        instance_name: Instance name for RG convergence plot
    """
    try:
        import matplotlib
        matplotlib.use("Agg")
        import matplotlib.pyplot as plt
        import numpy as np
    except ImportError:
        print("\n[!] matplotlib/numpy not installed. Skipping figure generation.")
        print("    Install with: pip install matplotlib numpy")
        return
    
    os.makedirs(out_dir, exist_ok=True)
    
    print("\n" + "="*80)
    print("  GENERATING FIGURES")
    print("="*80 + "\n")
    
    # Load CSV data
    if not os.path.exists(csv_path):
        print(f"[!] CSV file not found: {csv_path}")
        return
    
    with open(csv_path) as f:
        rows = list(csv.DictReader(f))
    
    if not rows:
        print("[!] No data in CSV file")
        return
    
    print(f"Loaded {len(rows)} rows from {csv_path}\n")
    
    # Algorithm metadata for plotting
    ALGO_LABELS = {
        "toyoda": "Toyoda",
        "improved_toyoda": "Modified Toyoda",
        "randomized_greedy": "Randomized Greedy",
        "improved_randomized_greedy": "Modified Randomized Greedy",
    }
    
    ALGO_COLORS = {
        "toyoda": "#4C72B0",
        "improved_toyoda": "#55A868",
        "randomized_greedy": "#DD8452",
        "improved_randomized_greedy": "#C44E52",
    }
    
    # Figure 1: Runtime vs n
    print("[1/5] Generating runtime comparison chart...")
    _plot_runtime_vs_n(rows, out_dir, ALGO_LABELS, ALGO_COLORS)
    
    # Figure 2: Profit comparison
    print("[2/5] Generating profit comparison chart...")
    _plot_profit_comparison(rows, out_dir, ALGO_LABELS, ALGO_COLORS)
    
    # Figure 3: RG convergence
    print("[3/5] Generating RG convergence chart...")
    _plot_rg_convergence(instance_name, out_dir, ALGO_COLORS)
    
    # Figure 4: Optimality gaps
    print("[4/5] Generating optimality gaps chart...")
    _plot_optimality_gaps(rows, out_dir, ALGO_LABELS, ALGO_COLORS)
    
    # Figure 5: Improvement analysis
    print("[5/5] Generating improvement analysis chart...")
    _plot_improvements(rows, out_dir)
    
    print("\n" + "="*80)
    print(f"  FIGURES SAVED TO: {out_dir}")
    print("="*80 + "\n")


def _safe_float(v):
    """Safely convert value to float."""
    try:
        return float(v)
    except (TypeError, ValueError):
        return None


def _plot_runtime_vs_n(rows, out_dir, labels, colors):
    """Chart 1: Runtime vs problem size."""
    import matplotlib.pyplot as plt
    import numpy as np
    
    data = defaultdict(lambda: defaultdict(list))
    for r in rows:
        n = r.get("n")
        if not n:
            continue
        for algo, col in [
            ("toyoda", "toyoda_ms"),
            ("improved_toyoda", "improved_toyoda_ms"),
            ("randomized_greedy", "randomized_greedy_ms"),
            ("improved_randomized_greedy", "improved_randomized_greedy_ms"),
        ]:
            t = _safe_float(r.get(col))
            if t is not None:
                data[n][algo].append(t)
    
    ns = sorted(data.keys(), key=int)
    if not ns:
        print("   [!] No runtime data available")
        return
    
    x = np.arange(len(ns))
    width = 0.20
    fig, ax = plt.subplots(figsize=(10, 6))
    
    algos = ["toyoda", "improved_toyoda", "randomized_greedy", "improved_randomized_greedy"]
    for i, algo in enumerate(algos):
        means = [np.mean(data[n].get(algo, [])) if data[n].get(algo) else 0 for n in ns]
        offset = (i - len(algos) / 2 + 0.5) * width
        ax.bar(x + offset, means, width, label=labels[algo], 
               color=colors[algo], edgecolor="white", linewidth=0.4)
    
    ax.set_yscale("log")
    ax.set_xticks(x)
    ax.set_xticklabels([str(n) for n in ns])
    ax.set_xlabel("Number of Items (n)", fontsize=11)
    ax.set_ylabel("Runtime (ms, log scale)", fontsize=11)
    ax.set_title("Algorithm Runtime vs Problem Size", fontsize=13, fontweight="bold")
    ax.legend(fontsize=10)
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    
    path = os.path.join(out_dir, "chart1_runtime_vs_n.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"   Saved: {path}")


def _plot_profit_comparison(rows, out_dir, labels, colors):
    """Chart 2: Profit comparison by dataset."""
    import matplotlib.pyplot as plt
    import numpy as np
    
    data = defaultdict(lambda: defaultdict(list))
    for r in rows:
        n = r.get("n")
        m = r.get("m")
        alpha = r.get("alpha", "")
        if not alpha:
            continue
        label = f"n={n},m={m}\nα={alpha}"
        
        for algo, col in [
            ("toyoda", "toyoda_profit"),
            ("improved_toyoda", "improved_toyoda_profit"),
            ("randomized_greedy", "randomized_greedy_best"),
            ("improved_randomized_greedy", "improved_randomized_greedy_best"),
        ]:
            profit = _safe_float(r.get(col))
            if profit is not None:
                data[label][algo].append(profit)
    
    labels_list = sorted(data.keys())
    if not labels_list:
        print("   [!] No profit data available")
        return
    
    x = np.arange(len(labels_list))
    width = 0.20
    fig, ax = plt.subplots(figsize=(max(10, len(labels_list) * 2), 6))
    
    algos = ["toyoda", "improved_toyoda", "randomized_greedy", "improved_randomized_greedy"]
    for i, algo in enumerate(algos):
        means = [np.mean(data[lbl].get(algo, [])) if data[lbl].get(algo) else 0 
                 for lbl in labels_list]
        offset = (i - len(algos) / 2 + 0.5) * width
        ax.bar(x + offset, means, width, label=labels[algo],
               color=colors[algo], edgecolor="white", linewidth=0.4)
    
    ax.set_xticks(x)
    ax.set_xticklabels(labels_list, fontsize=9)
    ax.set_xlabel("Dataset", fontsize=11)
    ax.set_ylabel("Average Profit", fontsize=11)
    ax.set_title("Profit Comparison by Algorithm and Dataset", fontsize=13, fontweight="bold")
    ax.legend(fontsize=10)
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    
    path = os.path.join(out_dir, "chart2_profit_comparison.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"   Saved: {path}")


def _plot_rg_convergence(instance_name, out_dir, colors):
    """Chart 3: RG convergence analysis."""
    import matplotlib.pyplot as plt
    
    base = Path(__file__).parent
    matches = list(base.rglob(f"*{instance_name}*.dat"))
    if not matches:
        print(f"   [!] Instance file not found for '{instance_name}'")
        return
    
    inst = parse_instance(str(matches[0]))
    print(f"   Running convergence analysis on {inst.name} (n={inst.n}, m={inst.m})")
    
    # Run Toyoda for baseline
    _, toy_p, _ = toyoda(inst)
    
    # Run RG algorithms with more iterations
    n_iter = 50
    _, _, _, rg_all = randomized_greedy(inst, k=5, n_iter=n_iter, seed=42)
    _, _, _, imp_rg_all = improved_randomized_greedy(inst, k=5, alpha=0.3, n_iter=n_iter, seed=42)
    
    fig, ax = plt.subplots(figsize=(10, 6))
    
    # Plot best-so-far curves
    for profits, label, color in [
        (rg_all, "Randomized Greedy", colors["randomized_greedy"]),
        (imp_rg_all, "Modified Randomized Greedy", colors["improved_randomized_greedy"]),
    ]:
        best_so_far = [max(profits[:i+1]) for i in range(len(profits))]
        ax.plot(range(1, len(best_so_far)+1), best_so_far,
                color=color, linewidth=2, label=label, marker='o', 
                markersize=3, markevery=5)
    
    # Add Toyoda baseline
    ax.axhline(toy_p, color=colors["toyoda"], linestyle="--",
               linewidth=1.5, label=f"Toyoda ({toy_p:,})")
    
    ax.set_xlabel("Number of Restarts", fontsize=11)
    ax.set_ylabel("Best Profit Found", fontsize=11)
    ax.set_title(f"RG Convergence Analysis — {inst.name} (n={inst.n}, m={inst.m})",
                fontsize=13, fontweight="bold")
    ax.legend(fontsize=10)
    ax.grid(linestyle="--", alpha=0.4)
    
    path = os.path.join(out_dir, "chart3_rg_convergence.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"   Saved: {path}")


def _plot_optimality_gaps(rows, out_dir, labels, colors):
    """Chart 4: Optimality gaps comparison."""
    import matplotlib.pyplot as plt
    import numpy as np
    
    data = defaultdict(lambda: defaultdict(list))
    for r in rows:
        n = r.get("n")
        if not n:
            continue
        for algo, col in [
            ("toyoda", "toyoda_gap"),
            ("improved_toyoda", "improved_toyoda_gap"),
            ("randomized_greedy", "randomized_greedy_gap"),
            ("improved_randomized_greedy", "improved_randomized_greedy_gap"),
        ]:
            gap = _safe_float(r.get(col))
            if gap is not None:
                data[n][algo].append(gap)
    
    ns = sorted(data.keys(), key=int)
    if not ns:
        print("   [!] No optimality gap data available")
        return
    
    x = np.arange(len(ns))
    width = 0.20
    fig, ax = plt.subplots(figsize=(10, 6))
    
    algos = ["toyoda", "improved_toyoda", "randomized_greedy", "improved_randomized_greedy"]
    for i, algo in enumerate(algos):
        means = [np.mean(data[n].get(algo, [])) if data[n].get(algo) else 0 for n in ns]
        offset = (i - len(algos) / 2 + 0.5) * width
        ax.bar(x + offset, means, width, label=labels[algo],
               color=colors[algo], edgecolor="white", linewidth=0.4)
    
    ax.set_xticks(x)
    ax.set_xticklabels([str(n) for n in ns])
    ax.set_xlabel("Number of Items (n)", fontsize=11)
    ax.set_ylabel("Average Optimality Gap (%)", fontsize=11)
    ax.set_title("Optimality Gap Comparison", fontsize=13, fontweight="bold")
    ax.legend(fontsize=10)
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    
    path = os.path.join(out_dir, "chart4_optimality_gaps.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"   Saved: {path}")


def _plot_improvements(rows, out_dir):
    """Chart 5: Improvement analysis."""
    import matplotlib.pyplot as plt
    import numpy as np
    
    # Collect improvement data
    improvements = defaultdict(list)
    for r in rows:
        for key in ["improved_toyoda_vs_toyoda_pct", "improved_randomized_greedy_vs_randomized_greedy_pct"]:
            val = _safe_float(r.get(key))
            if val is not None:
                improvements[key].append(val)
    
    if not improvements:
        print("   [!] No improvement data available")
        return
    
    fig, ax = plt.subplots(figsize=(10, 6))
    
    labels_map = {
        "improved_toyoda_vs_toyoda_pct": "Modified Toyoda vs Toyoda",
        "improved_randomized_greedy_vs_randomized_greedy_pct": "Modified RG vs Randomized Greedy",
    }
    
    colors_map = {
        "improved_toyoda_vs_toyoda_pct": "#55A868",
        "improved_randomized_greedy_vs_randomized_greedy_pct": "#C44E52",
    }
    
    pos = np.arange(len(improvements))
    for i, (key, values) in enumerate(improvements.items()):
        bp = ax.boxplot([values], positions=[i], widths=0.6,
                        patch_artist=True, showmeans=True,
                        meanprops=dict(marker='D', markerfacecolor='red', markersize=6))
        for patch in bp['boxes']:
            patch.set_facecolor(colors_map[key])
            patch.set_alpha(0.7)
    
    ax.set_xticks(pos)
    ax.set_xticklabels([labels_map[k] for k in improvements.keys()])
    ax.set_ylabel("Improvement (%)", fontsize=11)
    ax.set_title("Performance Improvement Analysis", fontsize=13, fontweight="bold")
    ax.axhline(0, color='black', linestyle='-', linewidth=0.5, alpha=0.3)
    ax.grid(axis="y", linestyle="--", alpha=0.4)
    
    path = os.path.join(out_dir, "chart5_improvement_analysis.png")
    fig.tight_layout()
    fig.savefig(path, dpi=150)
    plt.close(fig)
    print(f"   Saved: {path}")


# ── Script Integration Helpers ────────────────────────────────────────────────

def _run_script(script_path, csv_path, fig_dir, label):
    """
    Run an external script as a subprocess, passing CSV and fig-dir via env.
    Failures are reported but do not abort the overall pipeline.
    """
    import subprocess
    script_path = Path(script_path)
    if not script_path.exists():
        print(f"[!] {label}: script not found at {script_path}, skipping.")
        return

    print(f"\n{'='*80}")
    print(f"  RUNNING: {label}")
    print(f"  Script : {script_path.name}")
    print(f"{'='*80}\n")

    env = os.environ.copy()
    env["MKP_CSV_PATH"] = str(csv_path)
    env["MKP_FIG_DIR"] = str(fig_dir)

    result = subprocess.run(
        [sys.executable, str(script_path)],
        env=env,
    )
    if result.returncode != 0:
        print(f"[!] {label}: exited with code {result.returncode}")


# ── Main ──────────────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description="Run full MKP experiment and generate all figures",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__)
    
    parser.add_argument("--quick", action="store_true",
                       help="Quick test mode (10 instances total in --all mode, 2/dataset otherwise)")
    parser.add_argument("--all", action="store_true",
                       help="Run on ALL datasets (chubeas, gk, sac94) - 300+ instances")
    parser.add_argument("--datasets", nargs="+",
                       default=None,
                       help="Specific datasets to run (default: OR5x100, OR10x250, OR30x500)")
    parser.add_argument("--out", default="results/full_results.csv",
                       help="Output CSV path")
    parser.add_argument("--fig-dir", default="results/figures",
                       help="Output directory for figures")
    parser.add_argument("--rg-iter", type=int, default=20,
                       help="RG iterations (default: 20)")
    parser.add_argument("--instance", default="OR5x100-0.25_1",
                       help="Instance for RG convergence plot")
    parser.add_argument("--skip-experiment", action="store_true",
                       help="Skip experiment, only generate figures from existing CSV")
    
    args = parser.parse_args()
    
    # Run experiment
    if not args.skip_experiment:
        if args.all:
            # Run on ALL datasets (chubeas, gk, sac94)
            run_experiment(None, args.out, quick=args.quick, rg_iter=args.rg_iter, 
                          use_all_datasets=True)
        else:
            # Run on selected datasets
            if args.datasets is None:
                args.datasets = DEFAULT_DATASETS
            
            selected = {k: ALL_DATASETS[k] for k in args.datasets if k in ALL_DATASETS}
            
            if not selected:
                print(f"Error: No valid datasets selected from {list(ALL_DATASETS.keys())}")
                sys.exit(1)
            
            run_experiment(selected, args.out, quick=args.quick, rg_iter=args.rg_iter,
                          use_all_datasets=False)
    
    # Generate basic 5 charts
    generate_figures(args.out, args.fig_dir, args.instance)

    # Run Toyoda comparison analysis
    _run_script(Path(__file__).parent / "experiments" / "compare_toyoda.py",
                args.out, args.fig_dir, "Toyoda Comparison")

    # Run Randomized Greedy comparison analysis
    _run_script(Path(__file__).parent / "experiments" / "compare_randomized_greedy.py",
                args.out, args.fig_dir, "Randomized Greedy Comparison")

    # Run publication-quality figures (14 figures across chubeas / gk / sac94)
    _run_script(Path(__file__).parent / "experiments" / "generate_figures.py",
                args.out, args.fig_dir, "Publication Figures")

    print("\n✓ All done!\n")


if __name__ == "__main__":
    main()
