#!/usr/bin/env python3
"""
MKP CLI entry point.

Subcommands:
  parse       -- parse and inspect an instance file
  run         -- run one algorithm on a dataset directory
  experiment  -- full or quick experiment run (4 algos, multiple datasets)
  plot        -- generate charts from existing results CSV

Algorithms:
  toyoda                      — Toyoda (ratio recomputed each step)
  improved_toyoda             — Improved Toyoda (dynamic + fill-up pass)
  randomized_greedy           — GRASP-style RG, k-RCL, multi-restart
  improved_randomized_greedy  — alpha-threshold RCL + dead-item pruning + elite seed

Examples:
  python main.py parse --file All-MKP-Instances/chubeas/OR5x100/OR5x100-0.25_1.dat
  python main.py run --algo toyoda --dataset OR5x100
  python main.py run --algo improved_randomized_greedy --dataset OR10x250
  python main.py experiment --quick
  python main.py plot
"""

import argparse
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))

from mkp.parser import parse_instance, load_dataset_dir
from mkp.metrics import optimality_gap


# ── Subcommand handlers ───────────────────────────────────────────────────────

def cmd_parse(args):
    inst = parse_instance(args.file)
    print(inst)
    print(f"  profits[:5]    : {inst.profits[:5].tolist()}")
    print(f"  weights[0,:5]  : {inst.weights[0, :5].tolist()}")
    print(f"  capacities     : {inst.capacities.tolist()}")


def cmd_run(args):
    base = Path("All-MKP-Instances")
    matches = list(base.rglob(args.dataset))
    if not matches:
        print(f"Dataset dir '{args.dataset}' not found under {base}")
        sys.exit(1)
    ds_dir = matches[0]

    instances = load_dataset_dir(str(ds_dir))
    print(f"Loaded {len(instances)} instances from {ds_dir}\n")

    algo = args.algo.lower().replace("-", "_")

    for inst in instances:
        t0 = time.perf_counter()

        if algo == "toyoda":
            from mkp.algorithms.toyoda import toyoda
            sol, profit, feasible = toyoda(inst)

        elif algo == "improved_toyoda":
            from mkp.algorithms.improved_toyoda import improved_toyoda
            sol, profit, feasible = improved_toyoda(inst)

        elif algo == "randomized_greedy":
            from mkp.algorithms.randomized_greedy import randomized_greedy
            sol, profit, feasible, _ = randomized_greedy(
                inst, k=args.rg_k, n_iter=args.rg_iter, seed=args.seed)

        elif algo == "improved_randomized_greedy":
            from mkp.algorithms.improved_randomized_greedy import improved_randomized_greedy
            sol, profit, feasible, _ = improved_randomized_greedy(
                inst, k=args.rg_k, alpha=args.rg_alpha,
                n_iter=args.rg_iter, seed=args.seed)

        else:
            print(f"Unknown algorithm: {args.algo}")
            sys.exit(1)

        ms = (time.perf_counter() - t0) * 1000
        gap = optimality_gap(profit, inst.known_optimum)
        gap_str = f"{gap:.2f}%" if gap == gap else "N/A"
        print(f"{inst.name}: profit={profit}, feasible={feasible}, "
              f"time={ms:.1f}ms, gap={gap_str}")


def cmd_experiment(args):
    from run_full_experiment import run_experiment, ALL_DATASETS
    datasets = {k: ALL_DATASETS[k] for k in args.datasets if k in ALL_DATASETS}
    run_experiment(datasets, args.out, quick=args.quick, rg_iter=args.rg_iter)


def cmd_plot(args):
    from experiments.plots import main as plots_main
    sys.argv = ["plots.py",
                "--csv", args.csv,
                "--out-dir", args.out_dir,
                "--instance", args.instance]
    plots_main()


# ── Argument parser ───────────────────────────────────────────────────────────

def build_parser():
    p = argparse.ArgumentParser(description="MKP solver CLI (4-algorithm comparison)")
    sub = p.add_subparsers(dest="cmd")

    # parse
    sp = sub.add_parser("parse", help="Inspect an instance file")
    sp.add_argument("--file", required=True)

    # run
    sp = sub.add_parser("run", help="Run one algorithm on a dataset")
    sp.add_argument("--algo", required=True,
                    choices=[
                        "toyoda",
                        "improved_toyoda",
                        "randomized_greedy",
                        "improved_randomized_greedy",
                    ])
    sp.add_argument("--dataset", default="OR5x100",
                    help="Dataset dir name (searched under All-MKP-Instances/)")
    sp.add_argument("--rg-iter", type=int, default=20,
                    help="Restarts for RG algorithms (default: 20)")
    sp.add_argument("--rg-k", type=int, default=5,
                    help="RCL size k for RG algorithms (default: 5)")
    sp.add_argument("--rg-alpha", type=float, default=0.3,
                    help="Alpha threshold for Modified RG (default: 0.3)")
    sp.add_argument("--seed", type=int, default=42,
                    help="Random seed for RG algorithms (default: 42)")

    # experiment
    sp = sub.add_parser("experiment", help="Full experiment run (4 algorithms)")
    sp.add_argument("--quick", action="store_true",
                    help="Run only 2 instances per dataset for testing")
    sp.add_argument("--datasets", nargs="+",
                    default=["OR5x100", "OR10x250", "OR30x500"])
    sp.add_argument("--out", default="results/full_results.csv")
    sp.add_argument("--rg-iter", type=int, default=20,
                    help="Restarts for DynRG and ModRG (default: 20)")

    # plot
    sp = sub.add_parser("plot", help="Generate charts from results CSV")
    sp.add_argument("--csv", default="results/full_results.csv")
    sp.add_argument("--out-dir", default="results/figures")
    sp.add_argument("--instance", default="OR5x100-0.25_1",
                    help="Instance name for RG convergence plot")

    return p


def main():
    parser = build_parser()
    args = parser.parse_args()
    if args.cmd is None:
        parser.print_help()
        sys.exit(0)

    dispatch = {
        "parse":      cmd_parse,
        "run":        cmd_run,
        "experiment": cmd_experiment,
        "plot":       cmd_plot,
    }
    dispatch[args.cmd](args)


if __name__ == "__main__":
    main()