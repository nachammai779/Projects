# Machine Learning — Project 2: Logistic Regression

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Algorithm](https://img.shields.io/badge/Algorithm-Logistic%20Regression-green)

> Logistic Regression classifier implemented from scratch in Python using gradient descent optimization.

---

## Overview

This project implements **Logistic Regression** from scratch in Python. Logistic Regression models the probability that a sample belongs to a class using the sigmoid function, and learns optimal weights via gradient descent.

---

## Files

| File | Description |
|------|-------------|
| Logistic Regression.py | Full logistic regression implementation |

---

## Algorithm

1. Initialize weights w = 0
2. Compute prediction: h(x) = sigmoid(w^T * x)
3. Compute gradient of log-likelihood
4. Update weights: w = w + learning_rate * gradient
5. Repeat for n iterations or until convergence

---

## Key Concepts

- Sigmoid activation function for probability estimation
- Cross-entropy loss and log-likelihood
- Gradient descent optimization
- Binary classification with probability output
- Decision boundary at P(y=1|x) = 0.5

---

## Requirements

```bash
pip install numpy
```

---

## Run

```bash
python "Logistic Regression.py"
```
