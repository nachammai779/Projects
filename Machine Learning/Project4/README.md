# Machine Learning — Project 4: Ridge and Lasso Regression

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Regularization](https://img.shields.io/badge/Technique-Regularization-red)

> Implementation and comparison of Ridge (L2) and Lasso (L1) regularized regression with 3-fold cross-validation for hyperparameter selection.

---

## Overview

This project implements and compares **Ridge Regression (L2)** and **Lasso Regression (L1)** from scratch in Python. Both methods add a regularization term to prevent overfitting. The project includes 3-fold cross-validation to select the optimal regularization parameter lambda.

---

## Files

| File | Description |
|------|-------------|
| Ridge Regression - Main.py | Main ridge regression implementation |
| Ridge Regression - own.py | Custom ridge regression from scratch |
| Ridge Regression - with 3 fold CV.py | Ridge with 3-fold cross-validation |
| Lasso8.py | Lasso regression implementation |
| cg.py | Conjugate gradient solver utility |

---

## Key Concepts

### Ridge Regression (L2)
- Adds penalty: lambda * ||w||^2 to loss function
- Shrinks all weights toward zero but not exactly to zero
- Closed-form solution: w = (X^T*X + lambda*I)^-1 * X^T*y

### Lasso Regression (L1)
- Adds penalty: lambda * ||w||_1 to loss function
- Performs feature selection by driving some weights to exactly zero
- Requires iterative optimization (coordinate descent or subgradient)

### 3-Fold Cross-Validation
- Splits data into 3 equal folds
- Trains on 2 folds, validates on 1
- Repeats for each fold and averages validation error
- Selects lambda minimizing average validation error

---

## Requirements

```bash
pip install numpy
```

---

## Run

```bash
python "Ridge Regression - Main.py"
python "Ridge Regression - with 3 fold CV.py"
python Lasso8.py
```
