# Machine Learning Projects

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=c%2B%2B&logoColor=white)
![NumPy](https://img.shields.io/badge/NumPy-013243?style=flat&logo=numpy&logoColor=white)

> Five projects implementing core machine learning algorithms from scratch — covering linear models, perceptrons, bias-variance analysis, and regularization.

---

## 📂 Projects Overview

| Project | Topic | Algorithm | Language |
|---------|-------|-----------|----------|
| [Project 1](Project1/) | Perceptron Learning Algorithm | PLA (from scratch) | C++ |
| [Project 2](Project2/) | Logistic Regression | Gradient descent | Python |
| [Project 3](Project3/) | Bias-Variance Tradeoff | Gbar & Eout estimation | Python |
| [Project 4](Project4/) | Ridge & Lasso Regression | Regularization + 3-fold CV | Python |
| [Project 5](Project5/) | Linear Regression | With and without regularization | Python |

---

## 🔹 Project 1 — Perceptron Learning Algorithm (PLA)

**Goal:** Implement the PLA from scratch in C++ to find a linear decision boundary.

**Concepts:** Online learning, linear separability, convergence guarantee

**File:** `pla5pt.cpp`

---

## 🔹 Project 2 — Logistic Regression

**Goal:** Implement logistic regression using gradient descent without using ML libraries.

**Concepts:** Sigmoid activation, binary cross-entropy loss, gradient descent optimization

**File:** `Logistic Regression.py`

---

## 🔹 Project 3 — Bias-Variance Tradeoff (Gbar & Eout)

**Goal:** Empirically estimate the bias-variance decomposition.

**Concepts:** Expected hypothesis (Gbar), bias, variance, out-of-sample error (Eout)

Add Machine Learning README with descriptions for all 5 projects
---

## 🔹 Project 4 — Ridge Regression & Lasso with Cross-Validation

**Goal:** Implement and compare Ridge and Lasso regularization with 3-fold cross-validation.

**Concepts:** L1 and L2 regularization, conjugate gradient method, cross-validation, hyperparameter tuning

**Files:** `Ridge Regression - Main.py`, `Ridge Regression - own.py`, `Ridge Regression - with 3 fold CV.py`, `Lasso8.py`, `cg.py`

---

## 🔹 Project 5 — Linear Regression

**Goal:** Compare linear regression with and without regularization.

**Concepts:** Least squares, overfitting, regularization effect, dataset creation

**Files:** `Linear Regression.py`, `Linear Regression without regularization.py`, `Project 5 Data Set Creation.py`

---

## ⚙️ Requirements

```bash
pip install numpy matplotlib
# For C++ projects: g++ compiler
```
