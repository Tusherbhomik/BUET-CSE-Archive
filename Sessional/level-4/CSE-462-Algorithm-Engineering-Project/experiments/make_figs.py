"""Regenerate key figures from full_results.csv (4 algorithms only).

Algorithms:
  Toyoda  — toyoda_profit / toyoda_ms
  Improved Toyoda — improved_toyoda_profit / improved_toyoda_ms
  Randomized Greedy      — randomized_greedy_best / randomized_greedy_ms
  Improved Randomized Greedy     — improved_randomized_greedy_best / improved_randomized_greedy_ms
"""
import csv, numpy as np, sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent.parent))
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.ticker as mticker

plt.rcParams.update({
    "font.family": "serif", "font.size": 10, "figure.dpi": 150,
    "axes.grid": True, "grid.alpha": 0.35, "grid.linestyle": "--",
})

C = {
    "toyoda": "#4C72B0",
    "improved_toyoda": "#2196F3",
    "randomized_greedy":     "#DD8452",
    "improved_randomized_greedy":     "#C44E52",
}

OUT = Path(__file__).parent.parent / "results" / "figures"
OUT.mkdir(parents=True, exist_ok=True)

rows = list(csv.DictReader(
    open(Path(__file__).parent.parent / "results" / "full_results.csv")))

def fl(v):
    try: return float(v)
    except: return None

chubeas = [r for r in rows
           if r["group"] == "chubeas" and r["alpha"] != ""
           and fl(r["toyoda_profit"])]
gk_r    = [r for r in rows
           if r["group"] == "gk" and r.get("known_opt") and fl(r.get("toyoda_profit"))]
sac_r   = [r for r in rows
           if r["group"] == "sac94" and r.get("known_opt") and fl(r.get("toyoda_profit"))]


# ── FIG 4: Optimality gap (GK + SAC-94) ──────────────────────────────────────
def fig04():
    gap_meta = [
        ("Toyoda",                     "toyoda_gap",                       C["toyoda"]),
        ("Modified Toyoda",            "improved_toyoda_gap",              C["improved_toyoda"]),
        ("Randomized Greedy",          "randomized_greedy_gap",            C["randomized_greedy"]),
        ("Modified Randomized Greedy", "improved_randomized_greedy_gap",   C["improved_randomized_greedy"]),
    ]

    fig, axes = plt.subplots(1, 2, figsize=(15, 5))
    for ax, grp, title, rot in [
        (axes[0], gk_r,  "GK Instances (n=100–150, known optimum)", 30),
        (axes[1], sac_r, "SAC-94 Instances (n=10–105, known optimum)", 45),
    ]:
        labels, algo_gaps = [], {k: [] for k, _, _ in gap_meta}
        for r in sorted(grp, key=lambda x: x["instance"]):
            opt = fl(r.get("known_opt"))
            if not opt: continue
            tmp, row_ok = {}, True
            for label, col, _ in gap_meta:
                v = fl(r.get(col))
                if v is None: row_ok = False; break
                tmp[label] = v
            if not row_ok: continue
            labels.append(r["instance"])
            for label, _, _ in gap_meta:
                algo_gaps[label].append(tmp[label])

        n_algos = len(gap_meta)
        x = np.arange(len(labels))
        w = 0.18
        for i, (label, _, color) in enumerate(gap_meta):
            offset = (i - n_algos / 2 + 0.5) * w
            ax.bar(x + offset, algo_gaps[label], w, label=label,
                   color=color, edgecolor="white", linewidth=0.4)

        ax.set_xticks(x)
        ax.set_xticklabels(labels, rotation=rot, ha="right", fontsize=7)
        ax.set_ylabel("Optimality Gap (%)"); ax.set_title(title); ax.legend(fontsize=8)

    fig.suptitle("Fig 4 — Optimality Gap from Known Optimum (4 Algorithms)", fontsize=11)
    fig.tight_layout()
    fig.savefig(OUT / "fig04_optimality_gap.png")
    plt.close(fig)
    print("Saved fig04")


# ── FIG 13: SAC-94 per-subset gap bars ───────────────────────────────────────
def fig13():
    gap_meta = [
        ("Toyoda",                     "toyoda_gap",                       C["toyoda"]),
        ("Modified Toyoda",            "improved_toyoda_gap",              C["improved_toyoda"]),
        ("Randomized Greedy",          "randomized_greedy_gap",            C["randomized_greedy"]),
        ("Modified Randomized Greedy", "improved_randomized_greedy_gap",   C["improved_randomized_greedy"]),
    ]
    subsets = sorted(set(r["dataset"] for r in sac_r))
    names, algo_means = [], {k: [] for k, _, _ in gap_meta}

    for ds in subsets:
        sub = [r for r in sac_r if r["dataset"] == ds]
        if not sub: continue
        names.append(f"{ds}\n(n={sub[0]['n']},m={sub[0]['m']})")
        for label, col, _ in gap_meta:
            vals = [fl(r.get(col)) for r in sub if fl(r.get(col)) is not None]
            algo_means[label].append(np.mean(vals) if vals else 0)

    n_algos = len(gap_meta)
    x = np.arange(len(names))
    w = 0.18
    fig, ax = plt.subplots(figsize=(13, 5))

    for i, (label, _, color) in enumerate(gap_meta):
        offset = (i - n_algos / 2 + 0.5) * w
        ax.bar(x + offset, algo_means[label], w, label=label,
               color=color, edgecolor="white", linewidth=0.4)

    ax.set_xticks(x); ax.set_xticklabels(names, fontsize=8)
    ax.set_ylabel("Avg Optimality Gap (%)"); ax.legend()
    ax.set_title("Fig 13 — SAC-94 Average Optimality Gap by Subset (4 Algorithms)")
    fig.tight_layout()
    fig.savefig(OUT / "fig13_sac94_gap.png")
    plt.close(fig)
    print("Saved fig13")


# ── FIG 14: ModRG vs DynToyoda scatter ───────────────────────────────────────
def fig14():
    valid = [r for r in chubeas
             if fl(r["improved_randomized_greedy_best"]) and fl(r["toyoda_profit"])]

    alpha_c = {"0.25": "#E74C3C", "0.50": "#F39C12", "0.75": "#27AE60"}
    fig, axes = plt.subplots(1, 2, figsize=(13, 5))

    ax = axes[0]
    for alpha, col in alpha_c.items():
        sub = [r for r in valid if r["alpha"] == alpha]
        ax.scatter([fl(r["toyoda_profit"]) for r in sub],
                   [fl(r["improved_randomized_greedy_best"])        for r in sub],
                   color=col, label=f"α={alpha}", alpha=0.75, s=35)

    lo = min(fl(r["toyoda_profit"]) for r in valid) * 0.985
    hi = max(fl(r["toyoda_profit"]) for r in valid) * 1.005
    ax.plot([lo, hi], [lo, hi], "k--", linewidth=1, label="Equal")
    ax.set_xlabel("Toyoda Profit")
    ax.set_ylabel("Modified Randomized Greedy Profit")
    ax.set_title("Modified Randomized Greedy vs Toyoda Profit (n=100–500)")
    ax.legend()

    ax2 = axes[1]
    gaps = [100 * (fl(r["improved_randomized_greedy_best"]) - fl(r["toyoda_profit"])) /
            fl(r["toyoda_profit"]) for r in valid]
    ax2.hist(gaps, bins=20, color=C["improved_randomized_greedy"], edgecolor="white", alpha=0.85)
    ax2.axvline(np.mean(gaps), color="black", linestyle="--",
                label=f"Mean = {np.mean(gaps):.3f}%")
    ax2.axvline(0, color="red", linestyle=":", linewidth=1.2, label="Equal (0%)")
    ax2.set_xlabel("Modified Randomized Greedy surplus over Toyoda (%)")
    ax2.set_ylabel("# Instances")
    ax2.set_title("Distribution: Modified Randomized Greedy Gain over Toyoda")
    ax2.legend()

    fig.suptitle("Fig 14 — Modified Randomized Greedy vs Toyoda on All Chubeas Instances", fontsize=11)
    fig.tight_layout()
    fig.savefig(OUT / "fig14_modrg_vs_dyntoyoda.png")
    plt.close(fig)
    print("Saved fig14")


# ── FIG 15: Win counts across all chubeas rows ───────────────────────────────
def fig15():
    win_keys = [
        ("Toyoda",                       "toyoda_profit"),
        ("Modified Toyoda",              "improved_toyoda_profit"),
        ("Randomized Greedy",            "randomized_greedy_best"),
        ("Modified Randomized Greedy",   "improved_randomized_greedy_best"),
    ]
    counts = {k: 0 for k, _ in win_keys}
    ties = 0

    for r in chubeas:
        profits = {k: fl(r[col]) for k, col in win_keys}
        profits = {k: v for k, v in profits.items() if v is not None}
        if not profits: continue
        best = max(profits.values())
        winners = [k for k, v in profits.items() if v == best]
        if len(winners) > 1: ties += 1
        else: counts[winners[0]] += 1

    names  = list(counts.keys()) + ["Tie"]
    values = list(counts.values()) + [ties]
    colors = [C["toyoda"], C["improved_toyoda"], C["randomized_greedy"], C["improved_randomized_greedy"], "gray"]

    fig, ax = plt.subplots(figsize=(7, 4))
    bars = ax.bar(names, values, color=colors, edgecolor="white")
    for bar, val in zip(bars, values):
        ax.text(bar.get_x() + bar.get_width() / 2,
                bar.get_height() + 1, str(val),
                ha="center", va="bottom", fontweight="bold")
    ax.set_ylabel("Number of Instances Won")
    ax.set_ylim(0, max(values) * 1.15)
    ax.set_title(
        f"Fig 15 — Win Count: Which Algorithm Gets Best Profit?\n({len(chubeas)} chubeas instances)")
    plt.xticks(rotation=10, ha="right")
    fig.tight_layout()
    fig.savefig(OUT / "fig15_win_counts.png")
    plt.close(fig)
    print("Saved fig15")


if __name__ == "__main__":
    fig04()
    fig13()
    fig14()
    fig15()
    print(f"\nFigures saved to {OUT}")