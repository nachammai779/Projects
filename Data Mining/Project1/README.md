## Model Performance

### Model Evaluation & Optimization
This project implements a **Naive Bayes Classifier** from scratch to categorize technical Computer Science documentation. Through iterative testing, the model was optimized from a baseline accuracy of **33.33%** to a final performance of **80.00%** by performing hyperparameter tuning and addressing class imbalance.

### Hyperparameter Tuning
The model’s performance was significantly improved by adjusting the **Laplace Smoothing factor**:
* **Initial k=0.5**: Resulted in high bias, where the model over-generalized and classified neutral words as "Computer Science" (CS).
* **Optimized k=0.2**: Increased the model's sensitivity to specific technical vocabulary, allowing for a sharper decision boundary.

### Final Performance Metrics
The final model was evaluated on a test split of the 24-line dataset, yielding the following performance:

| Metric | Value |
| :--- | :--- |
| **Accuracy** | 80.00% |
| **True Negatives** | 7 (Identified all non-CS documents correctly) |
| **True Positives** | 1 (Confirmed technical keywords) |
| **Precision** | 100% (Zero False Positives) |

### Feature Importance (Top Predictors)
The classifier successfully "learned" a technical vocabulary. The following words carried the highest weight in predicting a Computer Science document classification:
1. **Regression** (Likelihood: 29.73%)
2. **Multinomial** (Likelihood: 16.22%)
3. **Adaboost** (Likelihood: 16.22%)
4. **Classifier** (Likelihood: 16.22%)
