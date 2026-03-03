# Data Mining — Project 3: K-Means & Bisecting K-Means Clustering

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![Clustering](https://img.shields.io/badge/Algorithm-Clustering-orange)

> Implementation and comparison of K-Means and Bisecting K-Means clustering algorithms from scratch in Python.

---

## Overview

This project implements and compares two clustering algorithms: **K-Means** and **Bisecting K-Means**. Both are built from scratch without machine learning libraries. Bisecting K-Means produces hierarchically structured clusters compared to standard K-Means.

---

## Files

| File | Description |
|------|-------------|
| DM Proj 3.py | K-Means and Bisecting K-Means implementation |

---

## Algorithms

### K-Means
1. Initialize k centroids randomly
2. Assign each point to the nearest centroid
3. Recompute centroids as mean of assigned points
4. Repeat until convergence

### Bisecting K-Means
1. Start with all data as one cluster
2. Select the largest cluster
3. Apply K-Means (k=2) to bisect it
4. Repeat until k clusters are formed
5. Produces better cluster quality with hierarchical structure

---

## Key Concepts

- Unsupervised learning and clustering
- Euclidean distance for cluster assignment
- Centroid-based partitioning
- Within-cluster sum of squares (WCSS) for evaluation
- Bisecting approach reduces sensitivity to initialization

---

## Requirements

```bash
pip install numpy
```

---

## Run

```bash
python "DM Proj 3.py"
```
