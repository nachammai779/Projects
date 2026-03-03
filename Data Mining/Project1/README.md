# Data Mining — Project 1: Naive Bayes Text Classifier

![Python](https://img.shields.io/badge/Python-3776AB?style=flat&logo=python&logoColor=white)
![scikit-learn](https://img.shields.io/badge/scikit--learn-F7931E?style=flat&logo=scikit-learn&logoColor=white)
![Accuracy](https://img.shields.io/badge/Accuracy-80%25-brightgreen)

> Text classification using Bernoulli and Multinomial Naive Bayes to categorize Computer Science documentation. Achieved 80% accuracy with optimized Laplace smoothing (k=0.2).

---

## Goal

Implement and compare **Bernoulli Naive Bayes** and **Multinomial Naive Bayes** classifiers from scratch to classify text documents as Computer Science (CS) or non-CS. Iteratively tune hyperparameters to maximize classification accuracy.

---

## Files

| File | Description |
|------|-------------|
| `NaiveBayes.py` | Core Naive Bayes implementation |
| `CS5342_Proj1_BernoulliNB.py` | Bernoulli Naive Bayes classifier |
| `CS5342_Proj1_Multinomial NB.py` | Multinomial Naive Bayes classifier |
| `sample 1.py` | Sample data and test cases |

---

## Key Concepts

- **Bernoulli NB**: Binary feature model — word present (1) or absent (0)
- **Multinomial NB**: Frequency-based model — uses word counts as features
- **Laplace Smoothing**: Prevents zero probabilities; tuned k=0.2 for best performance
- **Class Imbalance**: Addressed via probability threshold adjustment

---

## Model Performance

### Model Evaluation & Optimization
This project implements a Naive Bayes Classifier from scratch to categorize technical Computer Science documentation. Through iterative testing, the model was optimized from a baseline accuracy of **33.33%** to a final performance of **80.00%** by performing hyperparameter tuning and addressing class imbalance.

### Hyperparameter Tuning
Performance was significantly improved by adjusting the **Laplace Smoothing factor**:
- **Initial k=0.5**: Resulted in high bias — model over-generalized and classified neutral words as CS
- **Optimized k=0.2**: Increased model sensitivity to specific technical vocabulary, allowing for a sharper decision boundary

### Final Performance Metrics

| Metric | Value |
|--------|-------|
| **Accuracy** | 80.00% |
| **True Negatives** | 7 (all non-CS documents correctly identified) |
| **True Positives** | 1 (confirmed technical keywords) |
| **Precision** | 100% (zero false positives) |

### Feature Importance (Top Predictors)
The classifier learned a technical vocabulary. The following words carried the highest weight in predicting CS classification:
1. **Regression** (Likelihood: 29.73%)
2. **Multinomial** (Likelihood: 16.22%)
3. **Adaboost** (Likelihood: 16.22%)
4. **Classifier** (Likelihood: 16.22%)

---

## Requirements

```bash
pip install numpy scikit-learn
```

---

## Run

```bash
python NaiveBayes.py
python CS5342_Proj1_BernoulliNB.py
python "CS5342_Proj1_Multinomial NB.py"
```
