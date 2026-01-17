# ==============================
# Imports
# ==============================

from collections import Counter, defaultdict
import math
import random
import re
import glob


# ==============================
# Utility Functions
# ==============================

def split_data(data, prob):
    """
    Splits the dataset into two parts based on probability.

    Args:
        data (list): List of data samples
        prob (float): Probability of a sample going into the first split

    Returns:
        tuple: (train_data, test_data)
    """
    results = [], []
    for row in data:
        # Randomly assign each row to train or test
        results[0 if random.random() < prob else 1].append(row)
    return results


def tokenize(message):
    """
    Converts a text message into a set of normalized tokens.

    Steps:
    - Lowercase the text
    - Extract alphanumeric words using regex
    - Remove duplicates by converting to a set

    Args:
        message (str): Input text

    Returns:
        set: Unique tokens from the message
    """
    message = message.lower()
    all_words = re.findall("[a-z0-9']+", message)
    return set(all_words)


# ==============================
# Training Utilities
# ==============================

def count_words(training_set):
    """
    Counts how many times each word appears in CS and non-CS documents.

    Args:
        training_set (list): List of (message, is_cs) tuples

    Returns:
        dict: word -> [cs_count, non_cs_count]
    """
    counts = defaultdict(lambda: [0, 0])

    for message, is_cs in training_set:
        for word in tokenize(message):
            # Increment CS count if is_cs=True, else non-CS count
            counts[word][0 if is_cs else 1] += 1

    return counts


def word_probabilities(counts, total_cs, total_non_cs, k=0.5):
    """
    Computes P(word | CS) and P(word | non-CS) with Laplace smoothing.

    Args:
        counts (dict): Word occurrence counts
        total_cs (int): Number of CS documents
        total_non_cs (int): Number of non-CS documents
        k (float): Smoothing factor

    Returns:
        list: (word, P(word|CS), P(word|non-CS))
    """
    return [
        (
            word,
            (cs + k) / (total_cs + 2 * k),
            (non_cs + k) / (total_non_cs + 2 * k)
        )
        for word, (cs, non_cs) in counts.items()
    ]


# ==============================
# Probability Computation
# ==============================

def cs_probability(word_probs, message):
    """
    Computes the probability that a message belongs to CS class
    using Naive Bayes with log probabilities.

    Args:
        word_probs (list): Learned word probabilities
        message (str): Input message

    Returns:
        float: P(CS | message)
    """
    message_words = tokenize(message)

    log_prob_if_cs = 0.0
    log_prob_if_not_cs = 0.0

    for word, prob_if_cs, prob_if_not_cs in word_probs:

        # If word appears in message, use P(word|class)
        if word in message_words:
            log_prob_if_cs += math.log(prob_if_cs)
            log_prob_if_not_cs += math.log(prob_if_not_cs)

        # Otherwise use P(not word|class)
        else:
            log_prob_if_cs += math.log(1.0 - prob_if_cs)
            log_prob_if_not_cs += math.log(1.0 - prob_if_not_cs)

    # Convert log probabilities back to normal scale
    prob_if_cs = math.exp(log_prob_if_cs)
    prob_if_not_cs = math.exp(log_prob_if_not_cs)

    # Normalize to get final probability
    return prob_if_cs / (prob_if_cs + prob_if_not_cs)


# ==============================
# Naive Bayes Classifier Class
# ==============================

class NaiveBayesClassifier:
    """
    Custom Naive Bayes text classifier.
    """

    def __init__(self, k=0.2):
        self.k = k                  # Laplace smoothing factor
        self.word_probs = []        # Learned word probabilities

    def train(self, training_set):
        """
        Trains the classifier using labeled training data.
        """
        # Count number of CS and non-CS documents
        num_cs = len([1 for _, is_cs in training_set if is_cs])
        num_non_cs = len(training_set) - num_cs

        # Build word frequency table
        word_counts = count_words(training_set)

        # Compute word probabilities
        self.word_probs = word_probabilities(
            word_counts,
            num_cs,
            num_non_cs,
            self.k
        )

    def classify(self, message):
        """
        Classifies a message and returns CS probability.
        """
        return cs_probability(self.word_probs, message)


# ==============================
# Data Loading
# ==============================

def get_subject_data(path):
    """
    Loads labeled documents from filesystem.

    Documents containing 'CSDOC' in filename are treated as CS.

    Args:
        path (str): File path with wildcard

    Returns:
        list: (line, is_cs) tuples
    """
    data = []
    files = glob.glob(path)
    print(f"Files found: {len(files)}")  # DEBUG LINE 1
    for fn in files:
        is_cs = "CSDOC" in fn

        with open(fn, 'r', encoding='latin-1') as file:
            for line in file:
                data.append((line, is_cs))
    print(f"Total lines of data: {len(data)}")  # DEBUG LINE 2
    return data

# ==============================
# Helper Function
# ==============================

def p_cs_given_word(word_prob):
    """
    Computes P(CS | word) for sorting words by importance.
    """
    word, prob_if_cs, prob_if_not_cs = word_prob
    return prob_if_cs / (prob_if_cs + prob_if_not_cs)

def print_confusion_matrix(counts):
    tp = counts.get((True, True), 0)
    fp = counts.get((False, True), 0)
    fn = counts.get((True, False), 0)
    tn = counts.get((False, False), 0)

    # Calculate Metrics
    total = tp + fp + fn + tn
    accuracy = (tp + tn) / total if total > 0 else 0

    # Precision: When it says CS, how often is it right?
    precision = tp / (tp + fp) if (tp + fp) > 0 else 0

    # Recall: Out of all actual CS docs, how many did it find?
    recall = tp / (tp + fn) if (tp + fn) > 0 else 0

    print("\n" + "╔" + "═" * 38 + "╗")
    print("║       MODEL PERFORMANCE REPORT       ║")
    print("╠" + "═" * 38 + "╣")
    print(f"║ Metrics:                             ║")
    print(f"║  > Accuracy:  {accuracy:>7.2%}                ║")
    print(f"║  > Precision: {precision:>7.2%}                ║")
    print(f"║  > Recall:    {recall:>7.2%}                ║")
    print("╠" + "═" * 38 + "╣")
    print("║ CONFUSION MATRIX:                    ║")
    print(f"║              |  Pred CS  | Pred Not  ║")
    print(f"║  Actual CS   |    {tp:<5}  |    {fn:<5}  ║")
    print(f"║  Actual Not  |    {fp:<5}  |    {tn:<5}  ║")
    print("╚" + "═" * 38 + "╝\n")
# ==============================
# Training and Evaluation Pipeline
# ==============================

def train_and_test_model(path):
    """
    Complete pipeline:
    - Load data
    - Split into train/test
    - Train classifier
    - Evaluate predictions
    """

    data = get_subject_data(path)

    random.seed(0)
    train_data, test_data = split_data(data, 0.75)

    classifier = NaiveBayesClassifier()
    classifier.train(train_data)

    print("--- STARTING CLASSIFICATION ---")
    # Classify test data
    classified = []
    for text, is_cs in test_data:
        prob = classifier.classify(text)
        print(f"File Content: {text[:30]}... | Probability: {prob:.4f}")
        classified.append((text, is_cs, prob))
    # Confusion matrix counts
    print("\n--- FINAL COUNTS ---")
    counts = Counter(
        (is_cs, cs_probability > 0.5)
        for _, is_cs, cs_probability in classified)

    # Sort misclassifications
    classified.sort(key=lambda row: row[2])
    notcs_cs = [row for row in classified if not row[1]][-5:]
    cs_notcs = [row for row in classified if row[1]][:5]

    # Rank words by importance
    words = sorted(classifier.word_probs, key=p_cs_given_word)
    print("\n--- TOP WORDS INDICATING 'CS' ---")
    print(words[-5:])  # Shows the top 5 CS-leaning words

    print_confusion_matrix(counts)
    return counts, notcs_cs, cs_notcs, words

# ==============================
# Entry Point
# ==============================

if __name__ == "__main__":
    train_and_test_model(
        r"C:\Data Mining\Naive Bayes\*.txt"
    )
