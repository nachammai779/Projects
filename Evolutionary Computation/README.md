# Evolutionary Computation Projects

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![NumPy](https://img.shields.io/badge/NumPy-013243?style=flat&logo=numpy&logoColor=white)

> Three projects implementing evolutionary and genetic algorithms — covering evolutionary strategies, mutation operators, and constrained optimization with penalty functions.

---

## 📂 Projects Overview

| Project | Topic | Key Concept |
|---------|-------|-------------|
| [Project 1](Project1/) | Evolutionary Algorithm | Population-based search |
| [Project 2](Project2/) | Mutation Operators | Self-adaptive mutation |
| [Project 3](Project3/) | Constrained Optimization | Penalty function comparison |

---

## 🔹 Project 1 — Evolutionary Algorithm

**Goal:** Implement a basic evolutionary algorithm for function optimization.

**Key Concepts:** Population initialization, parent selection, crossover, mutation, survival selection

**Files:** `EC Proj 1.py`, `EC Proj 1 - Output on 2-18.txt`

---

Add Evolutionary Computation README with descriptions for all 3 projects
**Goal:** Explore and compare different mutation strategies in evolutionary algorithms.

**Key Concepts:** Gaussian mutation, self-adaptive step sizes, mutation rate effects on convergence

**Files:** `EC Proj 2.py`, `mutation samples.py`

---

## 🔹 Project 3 — Constrained Optimization with Penalty Functions

**Goal:** Solve a constrained optimization problem using three different penalty strategies and compare their effectiveness.

**Strategies Compared:**
- **No Penalty:** Unconstrained optimization (ignores constraints)
- **Linear Static Penalty:** Fixed penalty applied proportional to constraint violation
- **Adaptive Penalty:** Penalty adjusts dynamically based on population feasibility

**Key Concepts:** Constraint handling, penalty function design, convergence behavior under constraints

**Files:** `EC Proj 3 - With No Penalty.py`, `EC Proj 3 - With Linear Static Penalty.py`, `EC Proj 3 - With Adaptive Penalty.py`, `EC Proj 3.py`

---

## ⚙️ Requirements

```bash
pip install numpy matplotlib
```
