"""
Summary statistics from full_results.csv.

Covers all 4 algorithms:
  Toyoda  | Improved Toyoda  | Randomized Greedy  | Improved Randomized Greedy
"""

import csv, numpy as np
from pathlib import Path

rows = list(csv.DictReader(
    open(Path(__file__).parent.parent / "results" / "full_results.csv")))

def fl(v):
    try: return float(v)
    except: return None

# ── Filter helpers ────────────────────────────────────────────────────────────
chubeas  = [r for r in rows
            if r["group"] == "chubeas" and r["alpha"] != ""
            and fl(r["toyoda_profit"])]
gk_rows  = [r for r in rows if r["group"] == "gk"    and r.get("known_opt")]
sac_rows = [r for r in rows if r["group"] == "sac94" and r.get("known_opt")]

# ── Algorithm definitions ─────────────────────────────────────────────────────
ALGOS = [
    ("Toyoda",                     "toyoda_profit",                   "toyoda_ms"),
    ("Modified Toyoda",            "improved_toyoda_profit",          "improved_toyoda_ms"),
    ("Randomized Greedy",          "randomized_greedy_best",          "randomized_greedy_ms"),
    ("Modified Randomized Greedy", "improved_randomized_greedy_best", "improved_randomized_greedy_ms"),
]

# Head-to-head comparison columns (as produced by full_runner.py)
COMP_COLS = [
    ("Modified Toyoda vs Toyoda",              "improved_toyoda_vs_toyoda_pct"),
    ("Modified RG vs Randomized Greedy",       "improved_randomized_greedy_vs_randomized_greedy_pct"),
    ("Randomized Greedy vs Toyoda",            "randomized_greedy_vs_toyoda_pct"),
    ("Modified RG vs Modified Toyoda",         "improved_randomized_greedy_vs_improved_toyoda_pct"),
]

# Optimality gap columns
GAP_KEYS = [
    ("Toyoda",                     "toyoda_gap"),
    ("Modified Toyoda",            "improved_toyoda_gap"),
    ("Randomized Greedy",          "randomized_greedy_gap"),
    ("Modified Randomized Greedy", "improved_randomized_greedy_gap"),
]

# ─────────────────────────────────────────────────────────────────────────────
# 1. CHUBEAS — per-dataset summary
# ─────────────────────────────────────────────────────────────────────────────
print("=" * 90)
print("CHUBEAS SUMMARY — avg profit & runtime per dataset group (all α combined)")
print("=" * 90)

hdr = f"{'Dataset':12s}  " + "  ".join(f"{a[0]:>20s}" for a in ALGOS)
print(hdr)
print("-" * len(hdr))

for ds in sorted(set(r["dataset"] for r in chubeas)):
    sub = [r for r in chubeas if r["dataset"] == ds]
    cells = []
    for _, p_col, ms_col in ALGOS:
        p_vals  = [fl(r[p_col])  for r in sub if fl(r.get(p_col))]
        ms_vals = [fl(r[ms_col]) for r in sub if fl(r.get(ms_col))]
        avg_p  = np.mean(p_vals)  if p_vals  else 0
        avg_ms = np.mean(ms_vals) if ms_vals else 0
        cells.append(f"{avg_p:>10,.0f}({avg_ms:>7.1f}ms)")
    print(f"  {ds:12s}  " + "  ".join(cells))

# ─────────────────────────────────────────────────────────────────────────────
# 2. CHUBEAS — per-alpha breakdown
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("CHUBEAS — avg profit by tightness α (all datasets & sizes combined)")
print("=" * 90)

# Use DynToyoda as baseline for % improvement
for alpha in ["0.25", "0.50", "0.75"]:
    sub = [r for r in chubeas if r["alpha"] == alpha]
    base_vals = [fl(r["toyoda_profit"]) for r in sub if fl(r.get("toyoda_profit"))]
    base = np.mean(base_vals) if base_vals else 1
    parts = [f"α={alpha}:"]
    for label, p_col, _ in ALGOS:
        vals = [fl(r[p_col]) for r in sub if fl(r.get(p_col))]
        avg = np.mean(vals) if vals else 0
        imp = 100 * (avg - base) / base if base else 0
        sign = "+" if imp >= 0 else ""
        parts.append(f"{label}={avg:,.0f}({sign}{imp:.2f}%)")
    print("  " + "  ".join(parts))

# ─────────────────────────────────────────────────────────────────────────────
# 3. Head-to-head % comparisons
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("HEAD-TO-HEAD % COMPARISONS (all 270 chubeas instances)")
print("=" * 90)

for label, col in COMP_COLS:
    vals = [fl(r.get(col)) for r in chubeas if fl(r.get(col)) is not None]
    if vals:
        print(f"  {label}: mean={np.mean(vals):+.3f}%  "
              f"std={np.std(vals):.3f}%  "
              f"max={np.max(vals):+.3f}%  "
              f"min={np.min(vals):+.3f}%")

# ─────────────────────────────────────────────────────────────────────────────
# 4. Win count — which algorithm achieves highest profit?
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("WIN COUNT — which algorithm achieves the highest profit? (270 chubeas instances)")
print("=" * 90)

win_keys = [
    ("DynToyoda", "toyoda_profit"),
    ("ModToyoda", "improved_toyoda_profit"),
    ("DynRG",     "randomized_greedy_best"),
    ("ModRG",     "improved_randomized_greedy_best"),
]
counts = {k: 0 for k, _ in win_keys}
ties = 0

for r in chubeas:
    profits = {k: fl(r.get(col)) for k, col in win_keys}
    profits = {k: v for k, v in profits.items() if v is not None}
    if not profits:
        continue
    best = max(profits.values())
    winners = [k for k, v in profits.items() if v == best]
    if len(winners) > 1:
        ties += 1
    else:
        counts[winners[0]] += 1

for label, cnt in counts.items():
    print(f"  {label:12s}: {cnt:3d} wins")
print(f"  {'Tie':12s}: {ties:3d}")

# ─────────────────────────────────────────────────────────────────────────────
# 5. GK — optimality gaps
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("GK INSTANCES — optimality gap per algorithm (known optimum)")
print("=" * 90)

for r in sorted(gk_rows, key=lambda x: x["instance"]):
    gap_strs = []
    for label, col in GAP_KEYS:
        v = fl(r.get(col))
        gap_strs.append(f"{label}={v:.3f}%" if v is not None else f"{label}=N/A")
    print(f"  {r['instance']} (n={r['n']},m={r['m']}): " + "  ".join(gap_strs))

# Aggregate GK
print("\n  GK aggregate:")
for label, col in GAP_KEYS:
    vals = [fl(r.get(col)) for r in gk_rows if fl(r.get(col)) is not None]
    if vals:
        print(f"    {label:12s}: mean={np.mean(vals):.3f}%  "
              f"std={np.std(vals):.3f}%  min={np.min(vals):.3f}%")

# ─────────────────────────────────────────────────────────────────────────────
# 6. SAC-94 — per-subset optimality gaps
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("SAC-94 — average optimality gap by subset")
print("=" * 90)

for sub_ds in sorted(set(r["dataset"] for r in sac_rows)):
    sub = [r for r in sac_rows if r["dataset"] == sub_ds
           and fl(r.get("toyoda_profit"))]
    if not sub:
        continue
    n = sub[0]["n"]; m = sub[0]["m"]
    parts = [f"{sub_ds:8s} n={n:3s} m={m:2s}:"]
    for label, col in GAP_KEYS:
        vals = [fl(r.get(col)) for r in sub if fl(r.get(col)) is not None]
        avg = np.mean(vals) if vals else None
        parts.append(f"{label}={avg:.3f}%" if avg is not None else f"{label}=N/A")
    print("  " + "  ".join(parts))

# Aggregate SAC-94
print("\n  SAC-94 aggregate:")
for label, col in GAP_KEYS:
    vals = [fl(r.get(col)) for r in sac_rows if fl(r.get(col)) is not None]
    if vals:
        print(f"    {label:12s}: mean={np.mean(vals):.3f}%  "
              f"std={np.std(vals):.3f}%  min={np.min(vals):.3f}%")

# ─────────────────────────────────────────────────────────────────────────────
# 7. RG variance comparison (DynRG vs ModRG)
# ─────────────────────────────────────────────────────────────────────────────
print("\n" + "=" * 90)
print("RAND. GREEDY VARIANCE COMPARISON (avg std-dev across all chubeas instances)")
print("=" * 90)

for label, std_col in [
    ("DynRG",  "randomized_greedy_std"),
    ("ModRG",  "improved_randomized_greedy_std"),
]:
    vals = [fl(r.get(std_col)) for r in chubeas if fl(r.get(std_col)) is not None]
    if vals:
        print(f"  {label:8s}: avg_std={np.mean(vals):.2f}  "
              f"max_std={np.max(vals):.2f}")