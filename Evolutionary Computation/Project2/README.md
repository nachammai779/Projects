# Evolutionary Computation — Project 2: Mutation Operators

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Mutation-orange)

> Study and implementation of various mutation operators in evolutionary algorithms, comparing their effects on search behavior and convergence.

---

## Overview

This project investigates **mutation operators** in evolutionary computation. Mutation introduces genetic diversity into the population by randomly altering individual genes. Different mutation strategies have different effects on exploration vs exploitation behavior. The project implements and compares multiple mutation operators.

---

## Files

| File | Description |
|------|-------------|
| EC Proj 2.py | Main mutation operator experiments |
| mutation samples.py | Sample implementations of different mutation types |

---

## Mutation Operators Studied

### Bit-Flip Mutation (Binary)
- Randomly flip bits in a binary-encoded individual
- Mutation rate controls probability of each bit flipping

### Gaussian Mutation (Real-Valued)
- Add Gaussian noise: x_i = x_i + N(0, sigma)
- Sigma controls the step size of mutation

### Uniform Mutation
- Replace gene with random value from uniform distribution
- High diversity, less directional

### Adaptive Mutation
- Self-adapting mutation rates that evolve alongside the solution
- Allows the algorithm to adapt step sizes during search

---

## Key Concepts

- Exploration vs exploitation tradeoff
- Mutation rate effects on convergence speed and quality
- Self-adaptation of strategy parameters
- Diversity preservation in the population

---

## Requirements

```bash
pip install numpy matplotlib
```

---

## Run

```bash
python "EC Proj 2.py"
```
