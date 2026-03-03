# Evolutionary Computation — Project 1: Evolutionary Algorithm

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Evolutionary-orange)

> Implementation of a basic Evolutionary Algorithm (EA) with selection, crossover, and mutation operators for optimization.

---

## Overview

This project implements a fundamental **Evolutionary Algorithm (EA)** in Python. Evolutionary algorithms are population-based optimization techniques inspired by biological evolution. The algorithm iteratively evolves a population of candidate solutions toward better fitness through selection, crossover, and mutation.

---

## Files

| File | Description |
|------|-------------|
| EC Proj 1.py | Main evolutionary algorithm implementation |
| EC Proj 1 - Output on 2-18.txt | Sample output log from a run |

---

## Algorithm Flow

1. **Initialization**: Generate random population of candidate solutions
2. **Evaluation**: Compute fitness for each individual
3. **Selection**: Select parents using fitness-proportionate or tournament selection
4. **Crossover**: Combine pairs of parents to produce offspring
5. **Mutation**: Apply random perturbations to offspring
6. **Replacement**: Form new generation from offspring (and optionally parents)
7. **Repeat** until termination condition (max generations or fitness threshold)

---

## Key Concepts

- Population-based search and optimization
- Fitness landscape exploration vs exploitation
- Genetic operators: selection, crossover, mutation
- Generational replacement strategies
- Convergence and diversity in evolutionary search

---

## Requirements

```bash
pip install numpy
```

---

## Run

```bash
python "EC Proj 1.py"
```
