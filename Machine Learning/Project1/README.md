# Machine Learning — Project 1: Perceptron Learning Algorithm (PLA)

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Perceptron-blue)

> Perceptron Learning Algorithm (PLA) implemented in C++ to find a linear separator for a 2D dataset.

---

## Overview

This project implements the **Perceptron Learning Algorithm (PLA)** in C++. PLA is one of the simplest binary linear classifiers that learns a decision boundary by iteratively correcting misclassified samples.

---

## Files

| File | Description |
|------|-------------|
| pla5pt.cpp | PLA implementation in C++ |

---

## Algorithm

1. Initialize weight vector w = 0
2. For each misclassified point (x, y): update w = w + y*x
3. Repeat until no misclassified points remain
4. Output the final separating hyperplane

---

## Key Concepts

- Linear separability and decision boundaries
- Weight update rule for binary classification
- PLA convergence theorem
- Geometric interpretation: w^T * x = 0

---

## Build and Run

```bash
g++ -o pla pla5pt.cpp
./pla
```
