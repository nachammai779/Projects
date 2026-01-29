# AdaBoost From Scratch in Python

## Overview
This project implements the **AdaBoost (Adaptive Boosting)** algorithm entirely from scratch using Python.
Instead of relying on machine learning libraries, I built every component manually to deeply understand:

- weighted sampling
- weak learner boosting
- exponential loss minimization
- ensemble decision aggregation

The weak learner used in this implementation is a **decision stump**, making the learning process interpretable and transparent.

## Key Concepts Demonstrated
- Ensemble learning
- Boosting with adaptive weights
- Bias_variance tradeoff
- Weighted error minimization
- Model interpretability

## Algorithm Intuition
AdaBoost works by:
1. Starting with **uniform weights** for all training samples
2. Training a **weak learner**
3. Increasing weights of **misclassified samples**
4. Repeating this process to focus learning on harder examples
5. Combining weak learners into a **strong classifier**

