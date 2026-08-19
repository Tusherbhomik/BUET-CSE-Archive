#!/usr/bin/env python3
"""
Compare Randomized Greedy and Modified Randomized Greedy algorithms and generate visualizations.
Can be run standalone or called from run_full_experiment.py via env vars:
  MKP_CSV_PATH  — path to results CSV   (default: results/full_results.csv)
  MKP_FIG_DIR   — base figures dir      (default: results/figures)
"""

import os
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from pathlib import Path

# Set style
sns.set_style("whitegrid")
plt.rcParams['figure.figsize'] = (12, 8)

_BASE = Path(__file__).parent

# Resolve paths from env (set by run_full_experiment.py) or use defaults
csv_path   = os.environ.get("MKP_CSV_PATH",
             str(_BASE / "results" / "full_results.csv"))
_fig_base  = os.environ.get("MKP_FIG_DIR",
             str(_BASE / "results" / "figures"))

df = pd.read_csv(csv_path)

# Create output directory for figures
output_dir = Path(_fig_base) / "randomized_greedy_comparison"
output_dir.mkdir(exist_ok=True, parents=True)

print("="*80)
print("RANDOMIZED GREEDY vs MODIFIED RANDOMIZED GREEDY COMPARISON ANALYSIS")
print("="*80)

# Calculate improvement percentage (using best values)
df['profit_improvement'] = ((df['improved_randomized_greedy_best'] - df['randomized_greedy_best']) / df['randomized_greedy_best'] * 100)

better_count = (df['improved_randomized_greedy_best'] > df['randomized_greedy_best']).sum()
worse_count = (df['improved_randomized_greedy_best'] < df['randomized_greedy_best']).sum()
equal_count = (df['improved_randomized_greedy_best'] == df['randomized_greedy_best']).sum()

print(f"\nProfit Improvement (Best values):")
print(f"  Mean:   {df['profit_improvement'].mean():.3f}%")
print(f"  Median: {df['profit_improvement'].median():.3f}%")
print(f"  Wins/Losses/Ties: {better_count}/{worse_count}/{equal_count}")

print(f"\nRuntime:")
print(f"  Randomized Greedy Mean: {df['randomized_greedy_ms'].mean():.2f} ms")
print(f"  Improved Mean: {df['improved_randomized_greedy_ms'].mean():.2f} ms")
print(f"  Ratio: {(df['improved_randomized_greedy_ms'].mean() / df['randomized_greedy_ms'].mean()):.2f}x")

# Generate Graphs

# Graph 1: Bar Chart - Percentage Improvement by Dataset
plt.figure(figsize=(16, 8))
dataset_improvement = df.groupby('dataset').agg({
    'profit_improvement': 'mean',
    'instance': 'count'
}).reset_index()
dataset_improvement = dataset_improvement.sort_values('profit_improvement', ascending=False)

# Color bars based on positive/negative improvement
colors = ['green' if x >= 0 else 'red' for x in dataset_improvement['profit_improvement']]

bars = plt.bar(range(len(dataset_improvement)), dataset_improvement['profit_improvement'], 
               color=colors, alpha=0.7, edgecolor='black', linewidth=0.8)

plt.axhline(y=0, color='black', linestyle='-', linewidth=1)
plt.xlabel('Dataset', fontsize=13, fontweight='bold')
plt.ylabel('Average Improvement (%)', fontsize=13, fontweight='bold')
plt.title('Modified Randomized Greedy vs Randomized Greedy: Profit Improvement by Dataset', fontsize=15, fontweight='bold')
plt.xticks(range(len(dataset_improvement)), dataset_improvement['dataset'], rotation=90, ha='right', fontsize=9)
plt.grid(True, alpha=0.3, axis='y')

# Add value labels on bars
for i, (bar, val) in enumerate(zip(bars, dataset_improvement['profit_improvement'])):
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
             f'{val:.2f}%', ha='center', va='bottom' if height >= 0 else 'top', 
             fontsize=7, fontweight='bold')

plt.tight_layout()
plt.savefig(output_dir / 'improvement_by_dataset_bar.png', dpi=300, bbox_inches='tight')
print("\n✓ Saved: improvement_by_dataset_bar.png")
plt.close()

# Graph 2: Bar Chart - Percentage Improvement by Group (cleaner view)
plt.figure(figsize=(10, 6))
group_improvement = df.groupby('group').agg({
    'profit_improvement': 'mean',
    'instance': 'count'
}).reset_index()
group_improvement = group_improvement.sort_values('profit_improvement', ascending=False)

colors = ['#2ecc71' if x >= 0 else '#e74c3c' for x in group_improvement['profit_improvement']]
bars = plt.bar(group_improvement['group'], group_improvement['profit_improvement'], 
               color=colors, alpha=0.8, edgecolor='black', linewidth=1.5)

plt.axhline(y=0, color='black', linestyle='-', linewidth=2)
plt.xlabel('Dataset Group', fontsize=14, fontweight='bold')
plt.ylabel('Average Profit Improvement (%)', fontsize=14, fontweight='bold')
plt.title('Modified Randomized Greedy vs Randomized Greedy: Average Improvement by Dataset Group',
          fontsize=16, fontweight='bold', pad=20)
plt.grid(True, alpha=0.3, axis='y', linestyle='--')

# Add value labels on bars with instance count
for i, (bar, row) in enumerate(zip(bars, group_improvement.itertuples())):
    height = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2., height,
             f'{row.profit_improvement:.3f}%\n(n={row.instance})', 
             ha='center', va='bottom' if height >= 0 else 'top', 
             fontsize=12, fontweight='bold')

plt.tight_layout()
plt.savefig(output_dir / 'improvement_by_group_bar.png', dpi=300, bbox_inches='tight')
print("✓ Saved: improvement_by_group_bar.png")
plt.close()

# Graph 3: Individual Instance Improvement (Top 20 best and worst)
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(18, 8))

# Top 20 best improvements
top20 = df.nlargest(20, 'profit_improvement')[['instance', 'profit_improvement']].copy()
ax1.barh(range(len(top20)), top20['profit_improvement'], color='green', alpha=0.7, edgecolor='black')
ax1.set_yticks(range(len(top20)))
ax1.set_yticklabels(top20['instance'], fontsize=9)
ax1.set_xlabel('Improvement (%)', fontsize=12, fontweight='bold')
ax1.set_title('Top 20 Best Improvements', fontsize=13, fontweight='bold')
ax1.grid(True, alpha=0.3, axis='x')
ax1.invert_yaxis()

# Add values
for i, v in enumerate(top20['profit_improvement']):
    ax1.text(v, i, f' {v:.2f}%', va='center', fontsize=8, fontweight='bold')

# Top 20 worst improvements (most negative)
worst20 = df.nsmallest(20, 'profit_improvement')[['instance', 'profit_improvement']].copy()
ax2.barh(range(len(worst20)), worst20['profit_improvement'], color='red', alpha=0.7, edgecolor='black')
ax2.set_yticks(range(len(worst20)))
ax2.set_yticklabels(worst20['instance'], fontsize=9)
ax2.set_xlabel('Improvement (%)', fontsize=12, fontweight='bold')
ax2.set_title('Top 20 Worst Improvements', fontsize=13, fontweight='bold')
ax2.grid(True, alpha=0.3, axis='x')
ax2.invert_yaxis()

# Add values
for i, v in enumerate(worst20['profit_improvement']):
    ax2.text(v, i, f' {v:.2f}%', va='center', fontsize=8, fontweight='bold')

plt.tight_layout()
plt.savefig(output_dir / 'top_bottom_improvements.png', dpi=300, bbox_inches='tight')
print("✓ Saved: top_bottom_improvements.png")
plt.close()

# Graph 4: Runtime Analysis by Dataset Group
plt.figure(figsize=(12, 6))
group_runtime = df.groupby('group').agg({
    'randomized_greedy_ms': 'mean',
    'improved_randomized_greedy_ms': 'mean',
    'instance': 'count'
}).reset_index()

x = np.arange(len(group_runtime))
width = 0.35

bars1 = plt.bar(x - width/2, group_runtime['randomized_greedy_ms'], width,
                label='Randomized Greedy', alpha=0.8, color='#3498db', edgecolor='black', linewidth=1.5)
bars2 = plt.bar(x + width/2, group_runtime['improved_randomized_greedy_ms'], width,
                label='Modified Randomized Greedy', alpha=0.8, color='#e74c3c', edgecolor='black', linewidth=1.5)

plt.xlabel('Dataset Group', fontsize=14, fontweight='bold')
plt.ylabel('Average Execution Time (ms)', fontsize=14, fontweight='bold')
plt.title('Runtime Comparison: Randomized Greedy vs Modified Randomized Greedy', fontsize=16, fontweight='bold', pad=20)
plt.xticks(x, group_runtime['group'], fontsize=12)
plt.legend(fontsize=12)
plt.grid(True, alpha=0.3, axis='y', linestyle='--')

# Add value labels on bars
for bars in [bars1, bars2]:
    for bar in bars:
        height = bar.get_height()
        plt.text(bar.get_x() + bar.get_width()/2., height,
                 f'{height:.1f}', ha='center', va='bottom', fontsize=11, fontweight='bold')

plt.tight_layout()
plt.savefig(output_dir / 'runtime_comparison.png', dpi=300, bbox_inches='tight')
print("✓ Saved: runtime_comparison.png")
plt.close()

print("\n" + "="*80)
print(f"All graphs saved to: {output_dir}")
print("="*80)
print("\nCOMPARISON COMPLETE!")
