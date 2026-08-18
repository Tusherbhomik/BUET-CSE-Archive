# Multidimensional Knapsack Problem — Algorithm Comparison

> **CSE-462 Algorithm Lab · Part 2 · Omega Group 9**
> Implementation and experimental comparison of 4 heuristic algorithms on 335 OR-Library benchmark instances.

---

## Team

| Name | ID | Role |
|---|---|---|
| Shovon Roy | 2005010 | Algorithm Design & Toyoda |
| Tusher Bhomik | 2005046 | Modified Toyoda & Integration |
| Md. Rabby Hossain (Riyad) | 2005053 | Randomized Greedy & Experiments |
| Souvik Mandol | 2005069 | Modified Randomized Greedy & Parser |
| Sagor Chanda | 2005071 | Dataset Management & Metrics |
| Iffat Bin Hossain | 2005087 | Visualization & Report |

---

## Setup

```bash
pip install numpy matplotlib          # required
pip install pandas seaborn            # optional — for comparison charts
```

---

## Running Experiments

`run_full_experiment.py` is the **single entry point** for everything. It runs all 4 algorithms, writes results to CSV, and generates all figures in one command.

### Modes

| Command | What it does | Time |
|---|---|---|
| `python run_full_experiment.py` | Default 3 datasets (OR5x100, OR10x250, OR30x500) | ~15–40 min |
| `python run_full_experiment.py --quick` | 2 instances per dataset, smoke test | ~30 sec |
| `python run_full_experiment.py --all` | All 335 instances: chubeas + gk + sac94 | ~30–60 min |
| `python run_full_experiment.py --all --quick` | 10 instances total, fast sanity check | ~2 min |
| `python run_full_experiment.py --skip-experiment` | Regenerate all figures from existing CSV only | ~1 min |

### Select specific datasets

```bash
# Single dataset
python run_full_experiment.py --datasets OR5x100

# Any combination
python run_full_experiment.py --datasets OR5x100 OR10x250 OR30x500

# All n=100 variants (m = 5, 10, 30)
python run_full_experiment.py --datasets OR5x100 OR10x100 OR30x100

# All n=250 variants
python run_full_experiment.py --datasets OR5x250 OR10x250 OR30x250

# All n=500 variants
python run_full_experiment.py --datasets OR5x500 OR10x500 OR30x500
```

**Available dataset names:**

```
OR5x100   OR10x100   OR30x100
OR5x250   OR10x250   OR30x250
OR5x500   OR10x500   OR30x500
gk        sac94
```

### Tune RG iterations

```bash
python run_full_experiment.py --rg-iter 5      # fast, lower quality
python run_full_experiment.py --rg-iter 50     # recommended for final results
python run_full_experiment.py --rg-iter 100    # best quality, slower
```

### Custom output paths

```bash
python run_full_experiment.py --out results/my_results.csv --fig-dir results/my_figures
```

### Combining flags

```bash
# Specific datasets with more iterations
python run_full_experiment.py --datasets OR5x100 OR10x250 --rg-iter 50

# Full run with 50 iterations, custom output
python run_full_experiment.py --all --rg-iter 50 --out results/full_50iter.csv

# Quick test on a hard dataset
python run_full_experiment.py --datasets OR30x500 --quick

# Regenerate figures only (all dataset groups)
python run_full_experiment.py --all --skip-experiment
```

---

## Pipeline Output

Each run produces:

```
results/
├── full_results.csv
└── figures/
    ├── chart1_runtime_vs_n.png           runtime by problem size
    ├── chart2_profit_comparison.png      profit by algorithm and dataset
    ├── chart3_rg_convergence.png         best-of-N convergence curves
    ├── chart4_optimality_gaps.png        optimality gap (gk / sac94)
    ├── chart5_improvement_analysis.png   % improvement boxplots
    ├── toyoda_comparison/                4 figures: Toyoda vs Modified Toyoda
    ├── randomized_greedy_comparison/     4 figures: RG vs Modified RG
    ├── chubeas/                          11 publication figures
    ├── gk/                               3 publication figures
    └── sac94/                            2 publication figures
```

---

## Ad-hoc CLI (`main.py`)

Use `main.py` for quick inspection of a single instance or dataset without running the full pipeline.

### Inspect an instance file

```bash
python main.py parse --file All-MKP-Instances/chubeas/OR5x100/OR5x100-0.25_1.dat
```

```
MKPInstance(name='OR5x100-0.25_1', n=100, m=5, sum_profits=76842, known_opt=-1)
  profits[:5]   : [504, 803, 667, 1103, 834]
  weights[0,:5] : [42, 41, 523, 215, 819]
  capacities    : [11927, 13727, 11551, 13056, 13460]
```

### Run one algorithm on one dataset

```bash
python main.py run --algo toyoda --dataset OR5x100
python main.py run --algo improved_toyoda --dataset OR10x100
python main.py run --algo randomized_greedy --dataset OR10x250 --rg-iter 20
python main.py run --algo improved_randomized_greedy --dataset OR30x500 --rg-iter 50 --rg-alpha 0.3
```

`--algo` choices: `toyoda` · `improved_toyoda` · `randomized_greedy` · `improved_randomized_greedy`

### Run all 4 algorithms on selected datasets

```bash
python main.py experiment --quick
python main.py experiment --datasets OR5x100 OR10x250
python main.py experiment --datasets OR5x100 --out results/my_run.csv --rg-iter 20
```

### Generate basic charts from CSV

```bash
python main.py plot --csv results/full_results.csv --out-dir results/figures
python main.py plot --csv results/full_results.csv --out-dir results/figures --instance OR10x250-0.25_1
```

---

## Standalone Utilities

Run any of these independently after a CSV has been generated.

```bash
python experiments/stats.py                  # print summary statistics to terminal
python experiments/make_figs.py              # regenerate key figures subset
python experiments/generate_figures.py       # regenerate all 14 publication figures
python experiments/compare_toyoda.py        # regenerate Toyoda comparison figures
python experiments/compare_randomized_greedy.py  # regenerate RG comparison figures
```

---

## File Map

```
run_full_experiment.py          master pipeline — always use this
main.py                         ad-hoc CLI for single runs and inspection

mkp/
  instance.py                   MKPInstance dataclass
  parser.py                     OR-Library + SAC94 file parser
  metrics.py                    profit / feasibility / optimality gap
  algorithms/
    toyoda.py                   Toyoda
    improved_toyoda.py          Modified Toyoda
    randomized_greedy.py        Randomized Greedy
    improved_randomized_greedy.py   Modified Randomized Greedy

experiments/
  generate_figures.py           14 publication figures (called by master pipeline)
  plots.py                      3 basic charts (called by main.py plot)
  stats.py                      summary statistics printer
  make_figs.py                  key figures regenerator
  compare_toyoda.py             Toyoda vs Modified Toyoda analysis
  compare_randomized_greedy.py  RG vs Modified RG analysis

All-MKP-Instances/              335 benchmark .dat files
results/                        CSV output and generated figures
```

---

## Algorithms

### Problem

Maximize profit subject to _m_ resource constraints:

```
Maximize    Z = Σ pⱼ xⱼ
Subject to  Σ wᵢⱼ xⱼ ≤ Cᵢ    for all i = 1..m
            xⱼ ∈ {0, 1}
```

### Implementations

**Toyoda** · `O(n²·m)` · deterministic

Greedy selection using an efficiency ratio recomputed at each step with the *remaining* capacity:

```
ratio_j = p_j / Σᵢ ( w_ij / C_i_remaining )
```

---

**Modified Toyoda** · `O(n²·m)` · deterministic

Runs Toyoda, then a **fill-up pass** — tries every excluded item (by ratio order) and greedily inserts those that still fit. Always ≥ Toyoda at negligible extra cost.

---

**Randomized Greedy** · `O(N·n²·m)` · randomized

GRASP-style. At each step, picks uniformly at random from the **top-k Restricted Candidate List** instead of always taking the best. Repeats for `n_iter` independent restarts; returns the best. Default: `k=5`, `n_iter=20`, `seed=42`.

---

**Modified Randomized Greedy** · `O(N·n²·m)` · randomized

Enhances Randomized Greedy with:
- **Alpha-threshold RCL** — candidates must satisfy ratio ≥ α × best\_ratio (quality-based, not fixed-k)
- **Dead-item pruning** — drops items that can no longer fit before each step
- **Elite seed** — first restart is always the Modified Toyoda solution (deterministic floor)

Default: `k=5`, `alpha=0.3`, `n_iter=20`, `seed=42`.

---

| Algorithm | Complexity | Type | Notes |
|---|---|---|---|
| Toyoda | O(n²·m) | Deterministic | Baseline |
| Modified Toyoda | O(n²·m) | Deterministic | Always ≥ Toyoda |
| Randomized Greedy | O(N·n²·m) | Randomized | Best-of-N restarts |
| Modified Randomized Greedy | O(N·n²·m) | Randomized | Best overall |

---

## Dataset

**Source:** OR-Library — Chu & Beasley (1998). 335 valid individual instances across three families.

```
All-MKP-Instances/
├── chubeas/        9 groups: OR{m}x{n}  (m constraints, n items)
│   ├── OR5x100/    30 instances · α ∈ {0.25, 0.50, 0.75}
│   ├── OR10x100/
│   ├── OR30x100/
│   ├── OR5x250/
│   ├── OR10x250/
│   ├── OR30x250/
│   ├── OR5x500/
│   ├── OR10x500/
│   └── OR30x500/
├── gk/             Glover-Kochenberger instances · known optima available
└── sac94/          SAC-94 benchmarks · known optima available
```

**Constraint tightness α:**

| α | Difficulty |
|---|---|
| 0.25 | Tight — hard instances |
| 0.50 | Medium |
| 0.75 | Loose — easy instances |

---

*CSE-462 Algorithm Lab · Part 2 · Omega Group 9 · 2026*
