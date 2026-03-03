# Machine Learning — Project 3: Bias-Variance Analysis

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Analysis](https://img.shields.io/badge/Analysis-Bias--Variance-purple)

> Empirical analysis of the bias-variance tradeoff by computing Gbar (average hypothesis) and Eout (out-of-sample error) for various learning models.

---

## Overview

This project empirically measures the **bias-variance tradeoff** in machine learning models. By training multiple hypotheses on different data samples and computing the average hypothesis (Gbar) and expected out-of-sample error (Eout), the project quantifies how bias and variance contribute to generalization error.

---

## Files

| File | Description |
|------|-------------|
| GBAR and EOUT.py | Bias-variance analysis: computing Gbar and Eout |

---

## Key Concepts

### Bias-Variance Decomposition
- **Bias**: How far the average hypothesis (Gbar) is from the target function
- **Variance**: How much individual hypotheses vary around Gbar
- **Eout = Bias + Variance + Noise**

### Gbar (Average Hypothesis)
- Train many hypotheses on different random datasets
- Average them to get Gbar
- Gbar represents the "expected" model behavior

### Eout (Out-of-Sample Error)
- Evaluate each hypothesis on test data
- Average to get expected generalization error

---

## Requirements

```bash
pip install numpy matplotlib
```

---

## Run

```bash
python "GBAR and EOUT.py"
```
