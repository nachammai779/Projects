# Machine Learning — Project 5: Linear Regression

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Linear%20Regression-blue)

> Linear Regression implemented from scratch in Python, comparing regularized vs non-regularized approaches on a custom dataset.

---

## Overview

This project implements **Linear Regression** from scratch in Python, comparing performance with and without regularization. The project also includes a custom dataset generation script to evaluate model behavior on synthetic data.

---

## Files

| File | Description |
|------|-------------|
| Linear Regression.py | Linear regression with regularization |
| Linear Regression without regularization.py | Standard ordinary least squares (OLS) |
| Project 5 Data Set Creation.py | Custom synthetic dataset generator |

---

## Key Concepts

### Ordinary Least Squares (OLS)
- Minimizes sum of squared residuals: ||y - Xw||^2
- Closed-form solution: w = (X^T*X)^-1 * X^T*y
- No regularization — can overfit on noisy/small datasets

### Regularized Linear Regression
- Adds regularization term to prevent overfitting
- Improves generalization on unseen data
- Trades slightly higher training error for better test error

### Dataset Creation
- Generates synthetic regression data with configurable noise
- Enables controlled experiments to evaluate model behavior

---

## Requirements

```bash
pip install numpy matplotlib
```

---

## Run

```bash
python "Project 5 Data Set Creation.py"
python "Linear Regression.py"
python "Linear Regression without regularization.py"
```
