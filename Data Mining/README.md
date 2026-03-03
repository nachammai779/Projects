# Data Mining Projects

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![scikit-learn](https://img.shields.io/badge/scikit--learn-F7931E?style=flat&logo=scikit-learn&logoColor=white)

> Three projects implementing core data mining algorithms **from scratch** in Python — covering classification, ensemble learning, and clustering.

---

## 📂 Projects

| Project | Algorithm | Accuracy / Result |
|---------|-----------|-------------------|
| [Project 1](Project1/) | Naive Bayes Classifier (Bernoulli & Multinomial) | 80% accuracy on CS document classification |
| [Project 2](Project2/) | AdaBoost with Decision Stumps | Ensemble boosting from scratch |
| [Project 3](Project3/) | K-Means & Bisecting K-Means Clustering | Intra/inter cluster distance analysis |

---

## 🔹 Project 1 — Naive Bayes Classifier

**Goal:** Classify technical Computer Science documentation using Bernoulli and Multinomial Naive Bayes.

**Key Results:**
- Optimized Laplace smoothing from k=0.5 → k=0.2 to improve sensitivity
- Final accuracy: **80%** with 100% precision (zero false positives)
- Top predictors: Regression (29.73%), Multinomial (16.22%), Adaboost (16.22%)

**Files:** `NaiveBayes.py`, `CS5342_Proj1_BernoulliNB.py`, `CS5342_Proj1_Multinomial NB.py`

---

## 🔹 Project 2 — AdaBoost from Scratch

**Goal:** Implement the full AdaBoost (Adaptive Boosting) algorithm without ML libraries.

**Key Concepts:**
- Weighted sampling of training examples
- Decision stump weak learners
- Exponential loss minimization
- Adaptive weight update for misclassified samples

**Files:** `boosting.py`, `decisionstumps.py`

---

## 🔹 Project 3 — K-Means & Bisecting K-Means Clustering

**Goal:** Implement clustering algorithms from scratch and evaluate cluster quality.

**Key Concepts:**
- Standard K-Means with SSE and Manhattan distance modes
- Bisecting K-Means (recursive splitting of highest-SSE cluster)
- Intra-cluster distance (compactness) and inter-cluster distance (separation)
- Cluster visualization with matplotlib

**Files:** `DM Proj 3.py`

---

## ⚙️ Requirements

```bash
pip install numpy matplotlib
```
