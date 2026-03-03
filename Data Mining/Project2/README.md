# Data Mining — Project 2: AdaBoost from Scratch

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Ensemble Learning](https://img.shields.io/badge/Ensemble-AdaBoost-blue)

> AdaBoost (Adaptive Boosting) algorithm implemented entirely from scratch in Python using decision stumps as weak learners.

---

## Overview

This project implements the **AdaBoost (Adaptive Boosting)** algorithm entirely from scratch using Python. Instead of relying on machine learning libraries, every component is built manually to deeply understand weighted sampling, weak learner boosting, exponential loss minimization, and ensemble decision aggregation.

The weak learner used in this implementation is a **decision stump**, making the learning process interpretable and transparent.

---

## Files

| File | Description |
|------|-------------|
| boosting.py | Main AdaBoost implementation |
| decisionstumps.py | Decision stump weak learner |

---

## Algorithm Intuition

AdaBoost works by:
1. Starting with **uniform weights** for all training samples
2. Training a **weak learner** (decision stump)
3. Increasing weights of **misclassified samples**
4. Repeating this process to focus learning on harder examples
5. Combining weak learners into a **strong classifier**

---

## Key Concepts Demonstrated

- Ensemble learning and boosting
- Adaptive sample weighting
- Bias-variance tradeoff in boosted models
- Weighted error minimization
- Decision stumps as weak learners
- Model interpretability through simple base classifiers

---

## Requirements

```bash
pip install numpy
```

---

## Run

```bash
python boosting.py
```
