import random
import math
import matplotlib.pyplot as plt
import numpy as np
## Implementation Details
### 1. Weighted Sampling
###Samples are drawn based on a probability distribution that prioritizes harder examples.

def draw(weights):
    choice = random.uniform(0, sum(weights))
    choiceIndex = 0

    for weight in weights:
        choice -= weight
        if choice <= 0:
            return choiceIndex
        choiceIndex += 1

### 2. Weight Normalization
### Ensures weights sum to 1 after every update.

def normalize(weights):
    norm = sum(weights)
    return tuple(m / norm for m in weights)

def sign(x):
    return 1 if x >= 0 else -1

### 3. Weighted Error Computation
### Correct predictions contribute positively, incorrect ones increase error.
# boost: [(list, label)], learner, int -> (list -> label)
# where a learner is (() -> (list, label)) -> (list -> label)
# boost the weak learner into a strong learner

def computeError(h, examples, weights=None):
    if weights is None:
        weights = [1.] * len(examples)

    hypothesisResults = [h(x) * y for (x, y) in examples]
    return hypothesisResults, sum(w for (z, w) in zip(hypothesisResults, weights) if z < 0)

def boost(examples, individualLearner, rounds):
    weight_distr = normalize([1.] * len(examples))
    hypotheses = []
    alpha = []

    print('Initial weight distribution: ', weight_distr)

    for t in range(rounds):
        new_weight_distr = []
        print('Round: ', t)

        def drawExample():
            return examples[draw(weight_distr)]

        # 1. Get the learner and immediately append it
        current_h = individualLearner(drawExample)
        hypothesisResults, error = computeError(current_h, examples, weight_distr)

        # 2. Safety Check for perfect accuracy
        if error == 0:
            alpha.append(1.0) # Give it a standard weight
            hypotheses.append(current_h)
            print("Perfect learner found! Stopping early.")
            break

        # 3. Safety Check for poor learners
        if error >= 0.5:
            print(f"Weak learner error ({error:.3f}) >= 0.5. Skipping round.")
            continue

        # 4. Calculate and append alpha
        current_alpha = 0.5 * math.log((1 - error) / error)
        alpha.append(current_alpha)
        hypotheses.append(current_h)

        # 5. Update weights
        for i in range(len(examples)):
            if (hypothesisResults[i] >= 0):
                new_weight_distr.append(((0.5 * weight_distr[i]) / (1 - error)))
            else:
                new_weight_distr.append(((0.5 * weight_distr[i]) / error))
        weight_distr = new_weight_distr

        print('Alpha: ', current_alpha)
        print('Error: %.3f' % error)
# Inside the boost function's for loop:
        if t == 0 or t == rounds - 1:
            print(f"Visualizing weak learner for Round {t}...")
            visualize_stump(current_h, examples, round_num=t)
    def finalHypothesis(x):
        H = sum(a * h(x) for (a, h) in zip(alpha, hypotheses))
        return sign(H)
    return finalHypothesis

import matplotlib.pyplot as plt
import numpy as np

def visualize_boundary(h, train_data):
    # Extract training points and labels
    X_train = [x[0] for x, y in train_data]
    y_train = [y for x, y in train_data]

    # Create a dense range of points to see the boundary clearly
    # From 0 to 10 with 500 steps
    x_range = np.linspace(0, 10, 500)
    y_pred = [h([x]) for x in x_range]

    plt.figure(figsize=(10, 5))

    # Plot the decision region
    plt.fill_between(x_range, -1.2, 1.2, where=np.array(y_pred) == 1,
                     color='green', alpha=0.2, label='Predicted 1')
    plt.fill_between(x_range, -1.2, 1.2, where=np.array(y_pred) == -1,
                     color='red', alpha=0.2, label='Predicted -1')

    # Plot training points
    for x, y in train_data:
        color = 'green' if y == 1 else 'red'
        marker = 'o' if y == 1 else 'x'
        plt.scatter(x[0], y, c=color, marker=marker, s=100, edgecolors='k' if y==1 else None)

    plt.axhline(0, color='black', linewidth=0.5)
    plt.title('AdaBoost Final Decision Boundary')
    plt.xlabel('Feature Value')
    plt.ylabel('Label')
    plt.ylim(-1.5, 1.5)
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.show()

def visualize_stump(h_stump, train_data, round_num=0):
    # Color Palette
    COLOR_POS_REGION = '#4682B4' # Steel Blue
    COLOR_NEG_REGION = '#DB7093' # Pale Violet Red

    # Creating a dense range to find the single threshold
    x_range = np.linspace(0, 10, 1000)
    y_pred = [h_stump([x]) for x in x_range]

    plt.figure(figsize=(12, 4), facecolor='white')

    # 1. Fill regions to show the single split
    plt.fill_between(x_range, -1.1, 1.1, where=np.array(y_pred) == 1,
                     color=COLOR_POS_REGION, alpha=0.2, label='Stump +1')
    plt.fill_between(x_range, -1.1, 1.1, where=np.array(y_pred) == -1,
                     color=COLOR_NEG_REGION, alpha=0.2, label='Stump -1')

    # 2. Plot the training points
    for x, y in train_data:
        color = '#00FFFF' if y == 1 else '#FF00FF' # Cyan and Magenta
        marker = 'o' if y == 1 else 'x'
        plt.scatter(x[0], y, c=color, marker=marker, s=100, edgecolors='k' if y==1 else None, zorder=3)

    plt.axhline(0, color='black', linewidth=0.8, alpha=0.3)
    plt.title(f'Weak Learner Analysis: Decision Stump (Round {round_num})', fontsize=14)
    plt.xlabel('Feature Value')
    plt.ylabel('Label')
    plt.ylim(-1.5, 1.5)
    plt.grid(True, linestyle=':', alpha=0.4)
    plt.legend(loc='upper right')
    plt.show()

## Experimental Setup
### Training Data

def runMain():
   from decisionstumps import buildDecisionStump
   train = [([0.5], -1), ([3.0], -1), ([4.5], 1), ([4.6], 1), ([4.9], 1),
          ([5.2], -1), ([5.3], -1), ([5.5], 1), ([7.0], -1), ([9.5], -1)]
   test = [[1.0],[2.0],[3.0],[4.0],[5.0],[6.0],[7.0],[8.0],[9.0],[10.0]]
   individualLearner = buildDecisionStump
   rounds = 21

   h = boost(train, individualLearner, rounds)
   print(' ')
   print('Applying Hypothesis on Train Data: ',train)
   print(' ')
   print(f'Training errors: {computeError(h, train)}')
   print(' ')
   print('Applying Hypothesis on Test Data: ',test)
   print('Test Classification: ', [h(x) for x in test])

   print("Generating plot...")
   visualize_boundary(h, train)

if __name__ == "__main__":
   random.seed(1984)
   runMain()



