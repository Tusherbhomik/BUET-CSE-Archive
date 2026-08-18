import csv
import sys
from pathlib import Path
from collections import defaultdict

import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches   # noqa: F401  (available for callers)
import matplotlib.ticker

# ── Project root  (this file lives at <ROOT>/experiments/generate_figures.py)
BASE_DIR = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(BASE_DIR))

if not (BASE_DIR / "mkp").exists():
    print(f"INFO: 'mkp' package not found at {BASE_DIR / 'mkp'}  "
          "— fig03 (convergence) will be skipped.")

# ── I/O paths ─────────────────────────────────────────────────────────────────
# Respect env vars set by run_full_experiment.py, fall back to defaults
import os as _os
CSV      = Path(_os.environ.get("MKP_CSV_PATH",
           str(BASE_DIR / "results" / "full_results.csv")))
OUT_ROOT = Path(_os.environ.get("MKP_FIG_DIR",
           str(BASE_DIR / "results" / "figures")))

# Per-group output sub-directories
OUT = {
    "chubeas": OUT_ROOT / "chubeas",
    "gk":      OUT_ROOT / "gk",
    "sac94":   OUT_ROOT / "sac94",
    "general": OUT_ROOT / "general",
}
for d in OUT.values():
    d.mkdir(parents=True, exist_ok=True)

# ── Global plot style ─────────────────────────────────────────────────────────
plt.rcParams.update({
    "font.family":    "serif",
    "font.size":      10,
    "axes.titlesize": 11,
    "axes.labelsize": 10,
    "legend.fontsize": 9,
    "xtick.labelsize": 9,
    "ytick.labelsize": 9,
    "figure.dpi":     150,
    "axes.grid":      True,
    "grid.alpha":     0.35,
    "grid.linestyle": "--",
})

# ── Colour palette ────────────────────────────────────────────────────────────
C = {
    "toyoda": "#4C72B0",   # blue
    "improved_toyoda": "#2196F3",   # vivid blue
    "randomized_greedy":     "#DD8452",   # orange
    "improved_randomized_greedy":     "#C44E52",   # red
}

# (key, label, color, profit_col, ms_col)
ALGO_META = [
    ("toyoda", "Toyoda", C["toyoda"], "toyoda_profit", "toyoda_ms"),
    ("improved_toyoda", "Modified Toyoda", C["improved_toyoda"], "improved_toyoda_profit", "improved_toyoda_ms"),
    ("randomized_greedy",     "Randomized Greedy",    C["randomized_greedy"],     "randomized_greedy_best",       "randomized_greedy_ms"),
    ("improved_randomized_greedy",     "Modified Randomized Greedy",    C["improved_randomized_greedy"],     "improved_randomized_greedy_best",       "improved_randomized_greedy_ms"),
]

# Canonical chubeas ordering
DS_ORDER = [
    "OR5x100",  "OR10x100",  "OR30x100",
    "OR5x250",  "OR10x250",  "OR30x250",
    "OR5x500",  "OR10x500",  "OR30x500",
]

# ── Helpers ───────────────────────────────────────────────────────────────────
def fl(v):
    """Safely convert *v* to float; returns None on failure or NaN."""
    try:
        f = float(v)
        return f if not np.isnan(f) else None
    except (TypeError, ValueError):
        return None


def safe_get(row, col):
    """Float value for *col* in *row*, or None if missing / blank."""
    v = row.get(col, None)
    if v is None or str(v).strip() == "":
        return None
    return fl(v)


# ── Data loading ──────────────────────────────────────────────────────────────
def load_all_data():
    """
    Parse full_results.csv and split rows into per-group containers.

    Returns a dict with keys:
      all_rows      – every row with a valid baseline profit
      chubeas       – OR-Library chubeas rows
      gk_valid      – GK rows with a valid baseline profit
      gk_opt        – GK rows that also have a known optimum
      sac_valid     – SAC-94 rows with a valid baseline profit
      sac_opt       – SAC-94 rows that also have a known optimum
    Each group also carries a `<group>_datasets` list of sorted dataset names.
    """
    if not CSV.exists():
        print(f"CRITICAL: CSV not found at {CSV}")
        return {}

    with open(CSV, newline="", encoding="utf-8-sig") as fh:
        raw = list(csv.DictReader(fh))

    rows = [{k.strip(): str(v).strip() for k, v in r.items()} for r in raw]

    def has_baseline(r):
        return fl(r.get("toyoda_profit")) not in (None, 0)

    def has_opt(r):
        return any(r.get(k, "").strip() for k in ("known_opt", "optimum", "opt"))

    def group_of(r):
        return r.get("group", "").strip().lower()

    valid       = [r for r in rows if has_baseline(r)]
    chubeas     = [r for r in valid if group_of(r) == "chubeas"]
    gk_valid    = [r for r in valid if group_of(r) == "gk"]
    sac_valid   = [r for r in valid if group_of(r) == "sac94"]
    gk_opt      = [r for r in rows  if group_of(r) == "gk"    and has_opt(r)]
    sac_opt     = [r for r in rows  if group_of(r) == "sac94" and has_opt(r)]

    def ds_list(rlist):
        return sorted({r["dataset"] for r in rlist if r.get("dataset")})

    return {
        "all_rows":       valid,
        "chubeas":        chubeas,
        "chubeas_ds":     ds_list(chubeas),
        "gk_valid":       gk_valid,
        "gk_ds":          ds_list(gk_valid),
        "gk_opt":         gk_opt,
        "sac_valid":      sac_valid,
        "sac_ds":         ds_list(sac_valid),
        "sac_opt":        sac_opt,
    }


DATA = load_all_data()
if not DATA:
    raise SystemExit("Aborting: could not load data.")

# Convenience aliases used throughout
chubeas   = DATA["chubeas"]
gk_valid  = DATA["gk_valid"]
gk_opt    = DATA["gk_opt"]
sac_valid = DATA["sac_valid"]
sac_opt   = DATA["sac_opt"]

print(f"\nLoaded  →  total valid rows : {len(DATA['all_rows'])}")
print(f"  chubeas     : {len(chubeas):>4}  datasets: {DATA['chubeas_ds']}")
print(f"  gk (valid)  : {len(gk_valid):>4}  datasets: {DATA['gk_ds']}")
print(f"  gk (w/ opt) : {len(gk_opt):>4}")
print(f"  sac (valid) : {len(sac_valid):>4}  datasets: {DATA['sac_ds']}")
print(f"  sac (w/opt) : {len(sac_opt):>4}\n")


# ═════════════════════════════════════════════════════════════════════════════
# SHARED CHART BUILDERS  (called once per group)
# ═════════════════════════════════════════════════════════════════════════════

def _perf_gap_chart(rows, datasets, out_dir, group_label):
    """
    Horizontal grouped bar chart: % profit improvement vs Dyn. Toyoda.
    Saved to  <out_dir>/fig01_perf_gap.png
    """
    if not datasets:
        print(f"  Skipping fig01 [{group_label}]: no datasets.")
        return

    data = {ds: defaultdict(list) for ds in datasets}
    for r in rows:
        ds = r.get("dataset")
        if ds not in data:
            continue
        for _, _, _, p_col, _ in ALGO_META:
            v = safe_get(r, p_col)
            if v is not None:
                data[ds][p_col].append(v)

    baseline   = "toyoda_profit"
    n_algos    = len(ALGO_META)
    y_pos      = np.arange(len(datasets))
    bar_height = 0.2

    fig, ax = plt.subplots(figsize=(13, max(5, 0.55 * len(datasets) + 2)))

    for i, (key, label, color, p_col, _) in enumerate(ALGO_META):
        pct = []
        for ds in datasets:
            base = np.mean(data[ds][baseline]) if data[ds][baseline] else 0
            curr = np.mean(data[ds][p_col])   if data[ds][p_col]   else 0
            pct.append((curr - base) / base * 100 if base > 0 else 0)

        offset = (i - n_algos / 2 + 0.5) * bar_height
        bars = ax.barh(y_pos + offset, pct, bar_height,
                       label=label, color=color, edgecolor="black", linewidth=0.5)
        for bar in bars:
            w = bar.get_width()
            if abs(w) > 0.001:
                ax.annotate(
                    f"{w:+.2f}%",
                    xy=(w, bar.get_y() + bar.get_height() / 2),
                    xytext=(5, 0) if w >= 0 else (-5, 0),
                    textcoords="offset points",
                    ha="left" if w >= 0 else "right",
                    va="center", fontsize=7, fontweight="bold",
                )

    ax.axvline(0, color="black", linewidth=1, zorder=4)
    ax.set_yticks(y_pos)
    ax.set_yticklabels(datasets)
    ax.invert_yaxis()
    ax.set_xlabel("Profit Improvement vs. Toyoda (%)", fontweight="bold")
    ax.set_title(
        f"Performance Gap — {group_label}  ({len(datasets)} datasets)",
        fontweight="bold", pad=16,
    )
    xmin, xmax = ax.get_xlim()
    ax.set_xlim(xmin - 0.5, xmax + 1.5)
    ax.xaxis.set_major_formatter(matplotlib.ticker.PercentFormatter())
    ax.legend(loc="lower right", frameon=True, shadow=True, fontsize=9)
    plt.tight_layout()
    fig.savefig(out_dir / "fig01_perf_gap.png", bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig01 [{group_label}]  →  {out_dir / 'fig01_perf_gap.png'}")


def _runtime_chart(rows, datasets, out_dir, group_label):
    """
    Horizontal grouped bar chart: avg runtime on a log scale.
    Saved to  <out_dir>/fig02_runtime.png
    """
    if not datasets:
        print(f"  Skipping fig02 [{group_label}]: no datasets.")
        return

    data = {ds: defaultdict(list) for ds in datasets}
    for r in rows:
        ds = r.get("dataset")
        if ds not in data:
            continue
        for _, _, _, _, ms_col in ALGO_META:
            v = safe_get(r, ms_col)
            if v is not None:
                data[ds][ms_col].append(v)

    n_algos    = len(ALGO_META)
    y_pos      = np.arange(len(datasets))
    bar_height = 0.2

    fig, ax = plt.subplots(figsize=(13, max(5, 0.55 * len(datasets) + 2)))

    for i, (key, label, color, _, ms_col) in enumerate(ALGO_META):
        means  = [np.mean(data[ds][ms_col]) if data[ds][ms_col] else 1e-3
                  for ds in datasets]
        offset = (i - n_algos / 2 + 0.5) * bar_height
        ax.barh(y_pos + offset, means, bar_height,
                label=label, color=color, edgecolor="black", linewidth=0.5)

    ax.set_xscale("log")
    ax.set_yticks(y_pos)
    ax.set_yticklabels(datasets)
    ax.invert_yaxis()
    ax.set_xlabel("Average Runtime (ms, Log Scale)", fontweight="bold")
    ax.set_title(
        f"Average Runtime — {group_label}  ({len(datasets)} datasets)",
        fontweight="bold", pad=16,
    )
    ax.grid(True, which="both", ls="-", alpha=0.2)
    ax.legend(loc="lower right", frameon=True, shadow=True)
    plt.tight_layout()
    fig.savefig(out_dir / "fig02_runtime.png", bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig02 [{group_label}]  →  {out_dir / 'fig02_runtime.png'}")


def _optimality_gap_chart(rows_with_opt, out_dir, group_label):
    """
    Grouped bar chart: optimality gap (%) for instances with a known optimum.
    Saved to  <out_dir>/fig04_optimality_gap.png
    """
    if not rows_with_opt:
        print(f"  Skipping fig04 [{group_label}]: no rows with known optimum.")
        return

    inst_names = sorted({r.get("instance", r.get("dataset", "?"))
                         for r in rows_with_opt})
    x     = np.arange(len(inst_names))
    width = 0.2
    any_data = False

    fig, ax = plt.subplots(figsize=(max(10, len(inst_names) * 0.9 + 2), 5))

    for i, (key, label, color, p_col, _) in enumerate(ALGO_META):
        gap_col = f"{key}_gap"
        gaps = []
        for iname in inst_names:
            r = next((row for row in rows_with_opt
                      if row.get("instance", row.get("dataset")) == iname), None)
            val = safe_get(r, gap_col) if r else None
            if val is None and r is not None:
                opt = (safe_get(r, "optimum") or safe_get(r, "opt")
                       or safe_get(r, "known_opt"))
                pft = safe_get(r, p_col)
                if opt and pft and opt > 0:
                    val = (opt - pft) / opt * 100
            gaps.append(val if val is not None else 0)

        if any(g != 0 for g in gaps):
            any_data = True
        offset = (i - len(ALGO_META) / 2 + 0.5) * width
        ax.bar(x + offset, gaps, width, label=label, color=color,
               edgecolor="black", linewidth=0.5)

    if not any_data:
        ax.text(0.5, 0.5,
                "No gap data available.\n"
                "(Need '_gap' columns or 'optimum' column in CSV)",
                ha="center", va="center", transform=ax.transAxes,
                fontsize=10, color="gray",
                bbox=dict(boxstyle="round", facecolor="lightyellow", alpha=0.8))

    ax.set_xticks(x)
    ax.set_xticklabels(inst_names, rotation=45, ha="right", fontsize=8)
    ax.set_ylabel("Optimality Gap (%)", fontweight="bold")
    ax.set_title(
        f"Optimality Gap — {group_label}  ({len(inst_names)} instances)",
        fontweight="bold", pad=16,
    )
    ax.yaxis.set_major_formatter(matplotlib.ticker.PercentFormatter(decimals=1))
    ax.legend(frameon=True, shadow=True)
    plt.tight_layout()
    fig.savefig(out_dir / "fig04_optimality_gap.png", bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig04 [{group_label}]  →  {out_dir / 'fig04_optimality_gap.png'}")


# ═════════════════════════════════════════════════════════════════════════════
# GENERAL  (cross-dataset)
# ═════════════════════════════════════════════════════════════════════════════

def fig03_general():
    """
    DynRG / ModRG best-of-N convergence curves on 3 chubeas instances.
    Requires the 'mkp' package.
    Saved to: general/fig03_convergence.png
    """
    try:
        from mkp.parser import parse_instance
        from mkp.algorithms.randomized_greedy import \
            randomized_greedy as _drg
        from mkp.algorithms.improved_randomized_greedy import \
            improved_randomized_greedy as _mrg
        from mkp.algorithms.toyoda import toyoda as _dtoy
    except ImportError as exc:
        print(f"  Skipping fig03 [general]: mkp import failed — {exc}")
        return

    instances = [
        ("All-MKP-Instances/chubeas/OR5x100/OR5x100-0.25_1.dat",
         "OR5x100  (n=100, m=5)"),
        ("All-MKP-Instances/chubeas/OR10x250/OR10x250-0.25_1.dat",
         "OR10x250 (n=250, m=10)"),
        ("All-MKP-Instances/chubeas/OR30x500/OR30x500-0.25_1.dat",
         "OR30x500 (n=500, m=30)"),
    ]
    valid = [(fp, t) for fp, t in instances if (BASE_DIR / fp).exists()]
    if not valid:
        print("  Skipping fig03 [general]: no instance files found.")
        return

    fig, axes = plt.subplots(1, len(valid),
                             figsize=(5 * len(valid), 5), sharey=False)
    if len(valid) == 1:
        axes = [axes]

    for ax, (fpath, title) in zip(axes, valid):
        try:
            inst = parse_instance(str(BASE_DIR / fpath))
            _, dyn_toy_p, _ = _dtoy(inst)
            _, _, _, drg_all = _drg(inst, k=5, n_iter=50, seed=42)
            _, _, _, mrg_all = _mrg(inst, k=5, alpha=0.3, n_iter=50, seed=42)
        except Exception as exc:
            ax.text(0.5, 0.5, f"Error:\n{exc}",
                    ha="center", va="center", transform=ax.transAxes,
                    fontsize=8, color="red")
            ax.set_title(title, fontweight="bold")
            continue

        for profits, label, color in [
            (drg_all, "Randomized Greedy", C["randomized_greedy"]),
            (mrg_all, "Modified Randomized Greedy", C["improved_randomized_greedy"]),
        ]:
            bsf = [max(profits[:k + 1]) for k in range(len(profits))]
            ax.plot(range(1, len(bsf) + 1), bsf,
                    color=color, linewidth=2, label=label, zorder=3)

        ax.axhline(dyn_toy_p, color=C["toyoda"], linestyle="--",
                   linewidth=1.5, label="Toyoda Baseline", zorder=2)
        ax.set_title(title, fontweight="bold")
        ax.set_xlabel("Number of Restarts")
        ax.set_ylabel("Best Profit Found")
        ax.legend(fontsize=8, loc="lower right")
        ax.yaxis.set_major_formatter(
            matplotlib.ticker.StrMethodFormatter("{x:,.0f}"))

    fig.suptitle("Convergence: Best Profit vs. Number of Restarts  [General]",
                 fontsize=13, fontweight="bold", y=0.98)
    fig.tight_layout(rect=[0, 0.03, 1, 0.95])
    dest = OUT["general"] / "fig03_convergence.png"
    fig.savefig(dest)
    plt.close(fig)
    print(f"  Saved fig03 [general]  →  {dest}")


# ═════════════════════════════════════════════════════════════════════════════
# CHUBEAS  figures
# ═════════════════════════════════════════════════════════════════════════════

def fig05_chubeas():
    """
    Profit std-deviation of the RG variants vs problem size n.
    Saved to: chubeas/fig05_rg_stddev_vs_n.png
    """
    if not chubeas:
        print("  Skipping fig05 [chubeas]: no data.")
        return

    rg_variants = [
        ("Randomized Greedy", "randomized_greedy_best", "randomized_greedy_std", C["randomized_greedy"]),
        ("Modified Randomized Greedy", "improved_randomized_greedy_best", "improved_randomized_greedy_std", C["improved_randomized_greedy"]),
    ]
    n_vals = sorted({r.get("n", "") for r in chubeas if r.get("n")},
                    key=lambda v: int(v) if v.isdigit() else 0)
    if not n_vals:
        print("  Skipping fig05 [chubeas]: 'n' column not found.")
        return

    fig, ax = plt.subplots(figsize=(9, 5))
    for label, p_col, std_col, color in rg_variants:
        stds = []
        for n in n_vals:
            sub = [r for r in chubeas if r.get("n") == n]
            vals = [safe_get(r, std_col) for r in sub]
            vals = [v for v in vals if v is not None]
            if not vals:
                pv = [safe_get(r, p_col) for r in sub]
                pv = [v for v in pv if v is not None]
                vals = [np.std(pv)] if pv else [0]
            stds.append(np.mean(vals))

        ax.plot(n_vals, stds, "o-", color=color, linewidth=2,
                markersize=7, label=label, zorder=3)
        for n, s in zip(n_vals, stds):
            ax.annotate(f"{s:,.0f}", xy=(n, s), xytext=(0, 8),
                        textcoords="offset points",
                        ha="center", fontsize=8, color=color)

    ax.set_xlabel("Problem Size  n  (number of items)", fontweight="bold")
    ax.set_ylabel("Profit Std. Deviation", fontweight="bold")
    ax.set_title("RG Variant Profit Variability vs. Problem Size  [Chubeas]",
                 fontweight="bold", pad=15)
    ax.yaxis.set_major_formatter(
        matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))
    ax.legend(frameon=True, shadow=True)
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig05_rg_stddev_vs_n.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig05 [chubeas]  →  {dest}")


def fig06_chubeas():
    """
    Head-to-head % gain for 4 algorithm pairs, broken down by n.
    Saved to: chubeas/fig06_head_to_head.png
    """
    if not chubeas:
        print("  Skipping fig06 [chubeas]: no data.")
        return

    comp_meta = [
        ("Modified Toyoda\nvs Toyoda",
         "improved_toyoda_vs_toyoda_pct", "improved_toyoda_profit", "toyoda_profit",
         C["improved_toyoda"]),
        ("Modified RG\nvs\nRandomized Greedy",
         "improved_randomized_greedy_vs_randomized_greedy_pct", "improved_randomized_greedy_best", "randomized_greedy_best",
         C["improved_randomized_greedy"]),
        ("Randomized Greedy\nvs Toyoda",
         "randomized_greedy_vs_toyoda_pct", "randomized_greedy_best", "toyoda_profit",
         C["randomized_greedy"]),
        ("Modified RG\nvs\nModified Toyoda",
         "improved_randomized_greedy_vs_improved_toyoda_pct", "improved_randomized_greedy_best", "improved_toyoda_profit",
         C["improved_randomized_greedy"]),
    ]
    n_vals = sorted({r.get("n", "") for r in chubeas if r.get("n")},
                    key=lambda v: int(v) if v.isdigit() else 0)
    if not n_vals:
        print("  Skipping fig06 [chubeas]: 'n' column missing.")
        return

    fig, axes = plt.subplots(1, 4, figsize=(18, 5), sharey=False)
    for ax, (title, pct_col, new_col, base_col, color) in zip(axes, comp_meta):
        means = []
        for n in n_vals:
            sub  = [r for r in chubeas if r.get("n") == n]
            vals = [safe_get(r, pct_col) for r in sub]
            vals = [v for v in vals if v is not None]
            if not vals:
                pairs = [(safe_get(r, new_col), safe_get(r, base_col))
                         for r in sub]
                vals  = [(nv - bv) / bv * 100
                         for nv, bv in pairs
                         if nv is not None and bv and bv > 0]
            means.append(np.mean(vals) if vals else 0)

        bars = ax.bar(n_vals, means, color=color,
                      edgecolor="black", linewidth=0.5, zorder=3)
        ax.axhline(0, color="black", linewidth=0.8, linestyle="--", zorder=2)
        ax.set_title(title, fontsize=9, fontweight="bold")
        ax.set_xlabel("n  (items)")
        ax.set_ylabel("Avg % Gain")
        ax.yaxis.set_major_formatter(matplotlib.ticker.PercentFormatter())
        for bar, val in zip(bars, means):
            ypos = bar.get_height() + 0.05 if val >= 0 else bar.get_height() - 0.15
            ax.text(bar.get_x() + bar.get_width() / 2, ypos,
                    f"{val:+.2f}%", ha="center", va="bottom",
                    fontsize=7, fontweight="bold")

    fig.suptitle("Head-to-Head % Gain Comparisons (by n)  [Chubeas]",
                 fontsize=12, fontweight="bold")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig06_head_to_head.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig06 [chubeas]  →  {dest}")


def fig07_chubeas():
    """
    Average profit vs. constraint tightness α for all 4 algorithms.
    Saved to: chubeas/fig07_profit_vs_alpha.png
    """
    if not chubeas:
        print("  Skipping fig07 [chubeas]: no data.")
        return

    alphas  = ["0.25", "0.50", "0.75"]
    present = {r.get("alpha", "").strip() for r in chubeas}
    alphas  = [a for a in alphas if a in present]
    if not alphas:
        print("  Skipping fig07 [chubeas]: 'alpha' values not found.")
        return

    fig, ax = plt.subplots(figsize=(9, 5))
    for key, label, color, p_col, _ in ALGO_META:
        means = []
        for a in alphas:
            vals = [safe_get(r, p_col)
                    for r in chubeas if r.get("alpha", "").strip() == a]
            vals = [v for v in vals if v is not None]
            means.append(np.mean(vals) if vals else 0)
        ax.plot(alphas, means, "o-", color=color, label=label,
                linewidth=2, markersize=7, zorder=3)
        for a, m in zip(alphas, means):
            ax.annotate(f"{m:,.0f}", xy=(a, m), xytext=(0, 8),
                        textcoords="offset points",
                        ha="center", fontsize=7, color=color)

    ax.set_xlabel("Constraint Tightness (α)", fontweight="bold")
    ax.set_ylabel("Average Profit", fontweight="bold")
    ax.set_title("Profit vs. Constraint Tightness  [Chubeas]",
                 fontweight="bold", pad=15)
    ax.yaxis.set_major_formatter(
        matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))
    ax.legend(frameon=True, shadow=True)
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig07_profit_vs_alpha.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig07 [chubeas]  →  {dest}")


def fig08_chubeas():
    """
    Heatmap: Mod. RG % gain over Dyn. Toyoda on the n × m benchmark grid.
    Saved to: chubeas/fig08_modrg_heatmap.png
    """
    if not chubeas:
        print("  Skipping fig08 [chubeas]: no data.")
        return

    ns = sorted({r.get("n", "") for r in chubeas if r.get("n")},
                key=lambda v: int(v) if v.isdigit() else 0)
    ms = sorted({r.get("m", "") for r in chubeas if r.get("m")},
                key=lambda v: int(v) if v.isdigit() else 0)
    if not ns or not ms:
        print("  Skipping fig08 [chubeas]: 'n' or 'm' column missing.")
        return

    heat = np.zeros((len(ms), len(ns)))
    for ri, m in enumerate(ms):
        for ci, n in enumerate(ns):
            sub  = [r for r in chubeas if r.get("n") == n and r.get("m") == m]
            vals = [safe_get(r, "improved_randomized_greedy_vs_toyoda_pct") for r in sub]
            vals = [v for v in vals if v is not None]
            if not vals:
                pairs = [(safe_get(r, "improved_randomized_greedy_best"),
                          safe_get(r, "toyoda_profit")) for r in sub]
                vals  = [(mr - dt) / dt * 100
                         for mr, dt in pairs
                         if mr is not None and dt and dt > 0]
            heat[ri, ci] = np.mean(vals) if vals else 0

    fig, ax = plt.subplots(figsize=(9, 4))
    im = ax.imshow(heat, aspect="auto", cmap="RdYlGn", vmin=-1, vmax=5)
    ax.set_xticks(range(len(ns)))
    ax.set_xticklabels([f"n={n}" for n in ns])
    ax.set_yticks(range(len(ms)))
    ax.set_yticklabels([f"m={m}" for m in ms])
    ax.set_xlabel("Items  n", fontweight="bold")
    ax.set_ylabel("Constraints  m", fontweight="bold")
    ax.set_title("Modified Randomized Greedy % Gain over Toyoda  (n × m Heatmap)  [Chubeas]",
                 fontweight="bold", pad=15)
    for ri in range(len(ms)):
        for ci in range(len(ns)):
            txt_c = "black" if abs(heat[ri, ci]) < 3 else "white"
            ax.text(ci, ri, f"{heat[ri, ci]:.1f}%",
                    ha="center", va="center", fontsize=9,
                    color=txt_c, fontweight="bold")
    plt.colorbar(im, ax=ax, label="% gain", shrink=0.85)
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig08_modrg_heatmap.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig08 [chubeas]  →  {dest}")


def fig09_chubeas():
    """
    Profit-scaling error-bar lines: avg profit ± std vs n, per m group.
    Saved to: chubeas/fig09_profit_scaling.png
    """
    if not chubeas:
        print("  Skipping fig09 [chubeas]: no data.")
        return

    ms = sorted({r.get("m", "") for r in chubeas if r.get("m")},
                key=lambda v: int(v) if v.isdigit() else 0)
    ns = sorted({r.get("n", "") for r in chubeas if r.get("n")},
                key=lambda v: int(v) if v.isdigit() else 0)
    if not ms or not ns:
        print("  Skipping fig09 [chubeas]: 'n' or 'm' columns missing.")
        return

    fig, axes = plt.subplots(1, len(ms),
                             figsize=(5 * len(ms), 5), sharey=False)
    if len(ms) == 1:
        axes = [axes]

    for ax, m in zip(axes, ms):
        for key, label, color, p_col, _ in ALGO_META:
            means, stds = [], []
            for n in ns:
                vals = [safe_get(r, p_col)
                        for r in chubeas
                        if r.get("n") == n and r.get("m") == m]
                vals = [v for v in vals if v is not None]
                means.append(np.mean(vals) if vals else 0)
                stds.append(np.std(vals)   if len(vals) > 1 else 0)
            ax.errorbar(ns, means, yerr=stds, fmt="o-",
                        color=color, label=label, linewidth=2,
                        capsize=4, markersize=5, zorder=3)
        ax.set_title(f"m = {m}", fontweight="bold")
        ax.set_xlabel("n  (items)")
        ax.set_ylabel("Average Profit (± std)")
        ax.legend(fontsize=7)
        ax.yaxis.set_major_formatter(
            matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))

    fig.suptitle("Profit Scaling with n  (per m, 4 algorithms)  [Chubeas]",
                 fontsize=12, fontweight="bold")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig09_profit_scaling.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig09 [chubeas]  →  {dest}")


def fig10_chubeas():
    """
    Box-plots: Dyn. Toyoda vs Mod. Toyoda profit distributions, 3 dataset sizes.
    Saved to: chubeas/fig10_toyoda_variants.png
    """
    if not chubeas:
        print("  Skipping fig10 [chubeas]: no data.")
        return

    toy_variants = [
        ("Toyoda", "toyoda_profit", C["toyoda"]),
        ("Modified Toyoda", "improved_toyoda_profit", C["improved_toyoda"]),
    ]
    present = {r.get("dataset") for r in chubeas}
    targets = [ds for ds in ["OR5x100", "OR10x250", "OR30x500"] if ds in present]
    if not targets:
        targets = sorted(present)[:3]

    fig, axes = plt.subplots(1, len(targets),
                             figsize=(5 * len(targets), 5), sharey=False)
    if len(targets) == 1:
        axes = [axes]

    for ax, ds in zip(axes, targets):
        sub      = [r for r in chubeas if r.get("dataset") == ds]
        box_data = [[safe_get(r, col) for r in sub
                     if safe_get(r, col) is not None]
                    for _, col, _ in toy_variants]
        labels   = [lbl for lbl, _, _ in toy_variants]
        colors   = [col for _, _, col in toy_variants]

        bp = ax.boxplot(box_data, patch_artist=True, widths=0.5)
        for patch, color in zip(bp["boxes"], colors):
            patch.set_facecolor(color)
            patch.set_alpha(0.75)
        ax.set_xticklabels(labels, fontsize=9)
        ax.set_title(ds, fontweight="bold")
        ax.set_ylabel("Profit")
        ax.yaxis.set_major_formatter(
            matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))

    fig.suptitle("Toyoda Variants: Toyoda vs. Modified Toyoda  [Chubeas]",
                 fontsize=12, fontweight="bold")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig10_toyoda_variants.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig10 [chubeas]  →  {dest}")


def fig11_chubeas():
    """
    Summary table image: avg profit, runtime, and % improvement per dataset.
    Saved to: chubeas/fig11_summary_table.png
    """
    if not chubeas:
        print("  Skipping fig11 [chubeas]: no data.")
        return

    present   = {r.get("dataset") for r in chubeas}
    active_ds = [ds for ds in DS_ORDER if ds in present]
    if not active_ds:
        print("  Skipping fig11 [chubeas]: no datasets match DS_ORDER.")
        return

    rows_data = []
    for ds in active_ds:
        sub = [r for r in chubeas if r.get("dataset") == ds]
        if not sub:
            continue
        row = [ds, sub[0].get("n", "?"), sub[0].get("m", "?")]
        base_vals = [safe_get(r, "toyoda_profit") for r in sub]
        base_p    = np.mean([v for v in base_vals if v is not None] or [0])
        for _, label, _, p_col, ms_col in ALGO_META:
            pv  = [safe_get(r, p_col)  for r in sub]
            msv = [safe_get(r, ms_col) for r in sub]
            pv  = [v for v in pv  if v is not None]
            msv = [v for v in msv if v is not None]
            avg_p  = np.mean(pv)  if pv  else 0
            avg_ms = np.mean(msv) if msv else 0
            imp    = (avg_p - base_p) / base_p * 100 if base_p else 0
            row   += [f"{avg_p:,.0f}", f"{avg_ms:.1f} ms",
                      f"{'+'if imp>=0 else''}{imp:.2f}%"]
        rows_data.append(row)

    if not rows_data:
        print("  Skipping fig11 [chubeas]: no rows to display.")
        return

    algo_cols = []
    for _, label, _, _, _ in ALGO_META:
        algo_cols += [f"{label}\nProfit", f"{label}\nRuntime",
                      f"{label}\nvs Toyoda"]
    cols = ["Dataset", "n", "m"] + algo_cols

    fig, ax = plt.subplots(figsize=(26, 1.5 + 0.55 * len(rows_data)))
    ax.axis("off")
    tbl = ax.table(cellText=rows_data, colLabels=cols,
                   cellLoc="center", loc="center")
    tbl.auto_set_font_size(False)
    tbl.set_fontsize(7.5)
    tbl.scale(1, 1.5)
    for j in range(len(cols)):
        tbl[(0, j)].set_facecolor("#2C3E7A")
        tbl[(0, j)].set_text_props(color="white", fontweight="bold")
    for i in range(1, len(rows_data) + 1):
        bg = "#EEF2FF" if i % 2 == 0 else "white"
        for j in range(len(cols)):
            tbl[(i, j)].set_facecolor(bg)

    ax.set_title("Summary: Avg Profit & Runtime  [Chubeas]",
                 fontsize=11, pad=12, fontweight="bold")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig11_summary_table.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig11 [chubeas]  →  {dest}")


def fig12_chubeas():
    """
    Left: Mod. RG vs Dyn. Toyoda scatter (coloured by α).
    Right: histogram of Mod. RG surplus over Dyn. Toyoda.
    Saved to: chubeas/fig12_modrg_vs_dyntoyoda.png
    """
    valid = [r for r in chubeas
             if safe_get(r, "improved_randomized_greedy_best") is not None
             and safe_get(r, "toyoda_profit") is not None]
    if not valid:
        print("  Skipping fig12 [chubeas]: insufficient data.")
        return

    alpha_c = {"0.25": "#E74C3C", "0.50": "#F39C12", "0.75": "#27AE60"}
    fig, axes = plt.subplots(1, 2, figsize=(13, 5))

    ax = axes[0]
    plotted = False
    for alpha, col in alpha_c.items():
        sub = [r for r in valid if r.get("alpha", "").strip() == alpha]
        if not sub:
            continue
        ax.scatter([safe_get(r, "toyoda_profit") for r in sub],
                   [safe_get(r, "improved_randomized_greedy_best") for r in sub],
                   color=col, label=f"α={alpha}", alpha=0.65, s=25, zorder=3)
        plotted = True
    if not plotted:
        ax.scatter([safe_get(r, "toyoda_profit") for r in valid],
                   [safe_get(r, "improved_randomized_greedy_best") for r in valid],
                   color=C["improved_randomized_greedy"], alpha=0.6, s=25)

    dyn_all = [safe_get(r, "toyoda_profit") for r in valid]
    lo, hi  = min(dyn_all) * 0.985, max(dyn_all) * 1.005
    ax.plot([lo, hi], [lo, hi], "k--", linewidth=1.2, label="Equal", zorder=2)
    ax.set_xlabel("Toyoda Profit", fontweight="bold")
    ax.set_ylabel("Modified Randomized Greedy Profit", fontweight="bold")
    ax.set_title("Modified Randomized Greedy vs. Toyoda Profit", fontweight="bold")
    ax.xaxis.set_major_formatter(
        matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))
    ax.yaxis.set_major_formatter(
        matplotlib.ticker.FuncFormatter(lambda v, _: f"{int(v):,}"))
    ax.legend(fontsize=8)

    ax2   = axes[1]
    gaps  = [(safe_get(r, "improved_randomized_greedy_best") - safe_get(r, "toyoda_profit"))
             / safe_get(r, "toyoda_profit") * 100
             for r in valid]
    mean_gap = np.mean(gaps)
    ax2.hist(gaps, bins=25, color=C["improved_randomized_greedy"], edgecolor="white", alpha=0.85)
    ax2.axvline(mean_gap, color="black", linestyle="--", linewidth=1.5,
                label=f"Mean = {mean_gap:.3f}%")
    ax2.axvline(0, color="red", linestyle=":", linewidth=1.2, label="Equal (0%)")
    ax2.set_xlabel("Modified Randomized Greedy surplus over Toyoda (%)", fontweight="bold")
    ax2.set_ylabel("# Instances", fontweight="bold")
    ax2.set_title("Distribution of Modified Randomized Greedy Gap", fontweight="bold")
    ax2.legend()

    fig.suptitle(
        f"Modified Randomized Greedy vs. Toyoda  ({len(valid)} instances)  [Chubeas]",
        fontsize=12, fontweight="bold")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig12_modrg_vs_dyntoyoda.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig12 [chubeas]  →  {dest}")


def fig14_chubeas():
    """
    Bar chart: how many instances does each algorithm win?
    Saved to: chubeas/fig14_win_counts.png
    """
    if not chubeas:
        print("  Skipping fig14 [chubeas]: no data.")
        return

    win_keys = [
        ("Toyoda",                       "toyoda_profit"),
        ("Modified Toyoda",              "improved_toyoda_profit"),
        ("Randomized Greedy",            "randomized_greedy_best"),
        ("Modified Randomized Greedy",   "improved_randomized_greedy_best"),
    ]
    counts = {k: 0 for k, _ in win_keys}
    ties   = 0

    for r in chubeas:
        profits = {k: safe_get(r, col) for k, col in win_keys}
        profits = {k: v for k, v in profits.items() if v is not None}
        if not profits:
            continue
        best    = max(profits.values())
        winners = [k for k, v in profits.items() if v == best]
        if len(winners) > 1:
            ties += 1
        else:
            counts[winners[0]] += 1

    names  = list(counts.keys()) + ["Tie"]
    values = list(counts.values()) + [ties]
    colors = [C["toyoda"], C["improved_toyoda"], C["randomized_greedy"], C["improved_randomized_greedy"], "#95A5A6"]

    fig, ax = plt.subplots(figsize=(9, 5))
    bars = ax.bar(names, values, color=colors, edgecolor="black",
                  linewidth=0.6, zorder=3)
    for bar, val in zip(bars, values):
        ax.text(bar.get_x() + bar.get_width() / 2,
                bar.get_height() + 0.5, str(val),
                ha="center", va="bottom", fontweight="bold", fontsize=10)
    ax.set_ylabel("Number of Instances Won", fontweight="bold")
    ax.set_ylim(0, max(values) * 1.18)
    ax.set_title(
        f"Win Count: Best Profit per Instance  ({len(chubeas)} instances)  [Chubeas]",
        fontweight="bold", pad=15)
    plt.xticks(rotation=10, ha="right")
    plt.tight_layout()
    dest = OUT["chubeas"] / "fig14_win_counts.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig14 [chubeas]  →  {dest}")


# ═════════════════════════════════════════════════════════════════════════════
# SAC-94  figures
# ═════════════════════════════════════════════════════════════════════════════

def fig13_sac94():
    """
    Grouped bar chart: avg optimality gap per SAC-94 subset (4 algorithms).
    Saved to: sac94/fig13_gap_by_subset.png
    """
    if not sac_opt:
        print("  Skipping fig13 [sac94]: no SAC-94 rows with a known optimum.")
        return

    gap_meta = [
        ("Toyoda",                       "toyoda_profit",                      "toyoda_gap",                          C["toyoda"]),
        ("Modified Toyoda",              "improved_toyoda_profit",             "improved_toyoda_gap",                 C["improved_toyoda"]),
        ("Randomized Greedy",            "randomized_greedy_best",             "randomized_greedy_gap",               C["randomized_greedy"]),
        ("Modified Randomized Greedy",   "improved_randomized_greedy_best",    "improved_randomized_greedy_gap",      C["improved_randomized_greedy"]),
    ]
    subsets = sorted({r.get("dataset") for r in sac_opt if r.get("dataset")})

    names      = []
    algo_means = {lbl: [] for lbl, _, _, _ in gap_meta}

    for ds in subsets:
        sub = [r for r in sac_opt if r.get("dataset") == ds]
        if not sub:
            continue
        names.append(f"{ds}\n(n={sub[0].get('n','?')}, m={sub[0].get('m','?')})")
        for lbl, p_col, gap_col, _ in gap_meta:
            vals = [safe_get(r, gap_col) for r in sub]
            vals = [v for v in vals if v is not None]
            if not vals:
                opt_key = next((k for k in ("known_opt", "optimum", "opt")
                                if any(r.get(k) for r in sub)), None)
                if opt_key:
                    vals = [(safe_get(r, opt_key) - safe_get(r, p_col))
                            / safe_get(r, opt_key) * 100
                            for r in sub
                            if safe_get(r, opt_key) and safe_get(r, p_col)
                            and safe_get(r, opt_key) > 0]
            algo_means[lbl].append(np.mean(vals) if vals else 0)

    if not names:
        print("  Skipping fig13 [sac94]: could not build gap data.")
        return

    x     = np.arange(len(names))
    width = 0.18
    fig, ax = plt.subplots(figsize=(14, 5))
    for i, (lbl, _, _, color) in enumerate(gap_meta):
        offset = (i - len(gap_meta) / 2 + 0.5) * width
        ax.bar(x + offset, algo_means[lbl], width,
               label=lbl, color=color, edgecolor="black", linewidth=0.5)
    ax.set_xticks(x)
    ax.set_xticklabels(names, fontsize=8)
    ax.set_ylabel("Avg Optimality Gap (%)", fontweight="bold")
    ax.set_title("Average Optimality Gap by Subset  [SAC-94]",
                 fontweight="bold", pad=15)
    ax.yaxis.set_major_formatter(matplotlib.ticker.PercentFormatter(decimals=2))
    ax.legend(frameon=True, shadow=True)
    plt.tight_layout()
    dest = OUT["sac94"] / "fig13_gap_by_subset.png"
    fig.savefig(dest, bbox_inches="tight")
    plt.close(fig)
    print(f"  Saved fig13 [sac94]   →  {dest}")


# ═════════════════════════════════════════════════════════════════════════════
# Entry point
# ═════════════════════════════════════════════════════════════════════════════
if __name__ == "__main__":

    # ── Chubeas ──────────────────────────────────────────────────────────────
    print("── Chubeas ─────────────────────────────────────────────────────")
    _perf_gap_chart(chubeas,  DATA["chubeas_ds"], OUT["chubeas"], "Chubeas")
    _runtime_chart( chubeas,  DATA["chubeas_ds"], OUT["chubeas"], "Chubeas")
    fig05_chubeas()
    fig06_chubeas()
    fig07_chubeas()
    fig08_chubeas()
    fig09_chubeas()
    fig10_chubeas()
    fig11_chubeas()
    fig12_chubeas()
    fig14_chubeas()

    # ── GK ───────────────────────────────────────────────────────────────────
    print("\n── GK ──────────────────────────────────────────────────────────")
    _perf_gap_chart(gk_valid, DATA["gk_ds"],  OUT["gk"],    "GK")
    _runtime_chart( gk_valid, DATA["gk_ds"],  OUT["gk"],    "GK")
    _optimality_gap_chart(gk_opt,             OUT["gk"],    "GK")

    # ── SAC-94 ───────────────────────────────────────────────────────────────
    print("\n── SAC-94 ──────────────────────────────────────────────────────")
    _perf_gap_chart(sac_valid, DATA["sac_ds"], OUT["sac94"], "SAC-94")
    _runtime_chart( sac_valid, DATA["sac_ds"], OUT["sac94"], "SAC-94")
    _optimality_gap_chart(sac_opt,             OUT["sac94"], "SAC-94")
    fig13_sac94()

    # # ── General ──────────────────────────────────────────────────────────────
    # print("\n── General ─────────────────────────────────────────────────────")
    # fig03_general()

    # print(f"\n✓  All figures saved under: {OUT_ROOT}")
    # for group, path in OUT.items():
    #     pngs = sorted(path.glob("*.png"))
    #     print(f"   {group:8s}  ({len(pngs)} files)  →  {path}")