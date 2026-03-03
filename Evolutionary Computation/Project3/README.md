# Evolutionary Computation — Project 3: Constrained Optimization with Penalty Functions

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Constrained%20Optimization-orange)

> Comparison of three penalty function approaches for handling constraints in evolutionary optimization: No Penalty, Linear Static Penalty, and Adaptive Penalty.

---

## Overview

This project implements and compares **three penalty function strategies** for handling constraints in evolutionary algorithms. Constrained optimization problems require solutions to satisfy certain conditions — penalty functions encode constraint violations as fitness penalties, guiding the search toward feasible regions.

---

## Files

| File | Description |
|------|-------------|
| EC Proj 3.py | Main constrained optimization experiment |
| EC Proj 3 - With No Penalty.py | Baseline — no constraint handling |
| EC Proj 3 - With Linear Static Penalty.py | Fixed linear penalty for violations |
| EC Proj 3 - With Adaptive Penalty.py | Penalty adapts over generations |

---

## Penalty Strategies

### No Penalty
- Constraints are ignored during optimization
- May produce infeasible solutions
- Baseline for comparison

### Linear Static Penalty
- Penalize infeasible solutions: f_penalized = f(x) + C * violation
- Penalty coefficient C is fixed throughout the run
- Simple but sensitive to the choice of C

### Adaptive Penalty
- Penalty coefficient evolves during the search
- Increases when population has too many feasible solutions
- Decreases when too many infeasible solutions dominate
- More robust — does not require manual tuning of penalty weight

---

## Key Concepts

- Constrained optimization and feasibility
- Penalty function methods for constraint handling
- Trade-off between exploring infeasible and feasible regions
- Self-adaptation of penalty parameters
- Comparison metrics: best fitness, constraint satisfaction rate

---

## Requirements

```bash
pip install numpy matplotlib
```

---

## Run

```bash
python "EC Proj 3 - With No Penalty.py"
python "EC Proj 3 - With Linear Static Penalty.py"
python "EC Proj 3 - With Adaptive Penalty.py"
```
