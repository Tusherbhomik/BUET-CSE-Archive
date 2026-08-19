# MKP Algorithm Comparison Lab Report

## Overview
This directory contains a comprehensive academic lab report analyzing four heuristic algorithms for the Multidimensional Knapsack Problem (MKP).

## Report Details
- **Title:** Comparative Analysis of Heuristic Algorithms for the Multidimensional Knapsack Problem
- **Authors:** Omega Group 9 (CSE-462 Algorithm Lab, Part 2)
- **Format:** PDF (18 pages)
- **Compiled:** April 4, 2026

## Contents
- `main.tex` - LaTeX source file (complete report)
- `references.bib` - BibTeX bibliography (5 references)
- `main.pdf` - **Compiled PDF report** (final deliverable)
- `figures/` - 7 selected publication-quality figures

## Report Structure
1. **Abstract** - Problem overview, methodology, key findings
2. **Introduction** - MKP definition, applications, research questions
3. **Methods** - Detailed algorithm descriptions with pseudocode
4. **Experimental Setup** - Dataset, metrics, methodology
5. **Results** - Comprehensive performance analysis with figures
6. **Discussion** - Research question answers, practical guidelines
7. **Conclusion** - Contributions and future work
8. **References** - Academic citations

## Algorithms Analyzed
1. **Toyoda** - Deterministic greedy with toyoda ratios
2. **Improved Toyoda** - Toyoda + local search + fill-up
3. **Randomized Greedy** - GRASP-style multi-restart
4. **Improved Randomized Greedy** - Alpha-threshold RCL + elite seeding

## Key Findings
- **Improved Randomized Greedy** wins 56.7% of 344 instances (best overall)
- **Randomized Greedy** offers best quality-runtime tradeoff (+0.17% profit, +17% time)
- **Improved Toyoda** marginal improvement over Toyoda (+0.07%)
- Randomized algorithms excel on tight constraints (α = 0.25)

## Experimental Data
- **Instances:** 344 from OR-Library (Chu & Beasley)
- **Problem sizes:** n ∈ {100, 250, 500}, m ∈ {5, 10, 30}
- **Tightness levels:** α ∈ {0.25, 0.50, 0.75}

## Compilation
To recompile the PDF:
```bash
pdflatex main.tex
bibtex main
pdflatex main.tex
pdflatex main.tex
```

## Figures Included
1. `fig14_win_counts.png` - Win rate distribution
2. `fig02_runtime_log_all.png` - Runtime comparison (log scale)
3. `fig03_convergence_curves.png` - RG convergence behavior
4. `fig06_head_to_head.png` - Pairwise algorithm comparison
5. `fig07_profit_vs_alpha.png` - Impact of constraint tightness
6. `fig09_profit_scaling.png` - Scalability with problem size
7. `fig12_modrg_vs_dyntoyoda.png` - Best vs. baseline comparison

## Citation
If using this work, please cite:
```
Omega Group 9 (2026). Comparative Analysis of Heuristic Algorithms 
for the Multidimensional Knapsack Problem. CSE-462 Algorithm Laboratory, 
Part 2. Khulna University of Engineering & Technology.
```

## Related Files
- Full experimental results: `../results/full_results.csv`
- Additional figures: `../results/figures/`
- Algorithm implementations: `../mkp/algorithms/`
- Dataset: `../All-MKP-Instances/`
