from collections import Counter, defaultdict
from nltk.corpus import stopwords
import math, random, re, glob

global kw
kw = ['algorithm','classifier','linear','programming','binary']
def split_data(data, prob):
    """split data into fractions [prob, 1 - prob]"""
    results = [], []
    for row in data:
        results[0 if random.random() < prob else 1].append(row)
    return results

def tokenize(message):
    message = message.lower()                       # convert to lowercase
    all_words = re.findall("[a-z0-9']+", message)   # extract the words
    return set(all_words)                           # remove duplicates


def count_words(training_set):
    global kw
    """training set consists of pairs (message, is_cs)"""
    counts = defaultdict(lambda: [0, 0])
    sw = stopwords.words('english')
    for message, is_cs in training_set:
        for word in tokenize(message):
            if (word in kw and word not in sw):
                #counts[word][0 if is_cs else 1] += 1  # This is MultinomialNB
                counts[word][0 if is_cs else 1] = 1  # This is BernoulliNB
    return counts

def word_probabilities(counts, total_cs, total_non_cs, k=1):
    """turn the word_counts into a list of triplets
    w, p(w | cs) and p(w | ~cs)"""
    return [(w,
             (cs + k) / (total_cs + 2 * k),
             (non_cs + k) / (total_non_cs + 2 * k))
             for w, (cs, non_cs) in counts.items()]

def cs_probability(word_probs, message):
    message_words = tokenize(message)
    log_prob_if_cs = log_prob_if_not_cs = 0.0

    for word, prob_if_cs, prob_if_not_cs in word_probs:

        # for each word in the message,
        # add the log probability of seeing it
        if word in message_words:
            log_prob_if_cs += math.log(prob_if_cs)
            log_prob_if_not_cs += math.log(prob_if_not_cs)

        # for each word that's not in the message
        # add the log probability of not seeing it
        else:
            log_prob_if_cs += math.log(1.0 - prob_if_cs)
            log_prob_if_not_cs += math.log(1.0 - prob_if_not_cs)

    prob_if_cs = math.exp(log_prob_if_cs)
    prob_if_not_cs = math.exp(log_prob_if_not_cs)
    print('prob_if_cs: ', prob_if_cs)
    print('prob_if_not_cs: ', prob_if_not_cs)
    print('cs_probability return value: ', (prob_if_cs / (prob_if_cs + prob_if_not_cs)))
    print(' ')
    return prob_if_cs / (prob_if_cs + prob_if_not_cs)


class NaiveBayesClassifier:

    def __init__(self, k=1):
        self.k = k
        self.word_probs = []

    def train(self, training_set):

        # count cs and non-cs messages
        num_cs = len([is_cs
                         for message, is_cs in training_set
                         if is_cs])
        num_non_cs = len(training_set) - num_cs
        # run training data through our "pipeline"
        word_counts = count_words(training_set)
        self.word_probs = word_probabilities(word_counts,
                                             num_cs,
                                             num_non_cs,
                                             self.k)

        print('num_cs: ',num_cs)
        print('num_non_cs: ', num_non_cs)
        print (' ')
        print('word_counts: ',word_counts)
        print (' ')
        print('self.word_probs: ',self.word_probs)
        print (' ')

    def classify(self, message):
        print('At Classify based on above self.word_probs: ')
        print('message: ',message)
        print (' ')

        return cs_probability(self.word_probs, message)


def get_subject_data(path):
    data = []
    # glob.glob returns every filename that matches the wildcarded path
    for fn in glob.glob(path):
        is_cs = "CSDOC" in fn
        with open(fn,'r',encoding='latin-1') as file:
            for line in file:
                data.append((line, is_cs))
    #print('data: ', data)
    return data

def p_cs_given_word(word_prob):
    word, prob_if_cs, prob_if_not_cs = word_prob
    return prob_if_cs / (prob_if_cs + prob_if_not_cs)

def train_and_test_model(path):

    data = get_subject_data(path)
    random.seed(0)
    train_data, test_data = split_data(data, 0.75)
    print('train_data: ', train_data)
    print('test_data: ', test_data)
    print(' ')
    classifier = NaiveBayesClassifier()
    classifier.train(train_data)

    classified = [(subject, is_cs, classifier.classify(subject))
              for subject, is_cs in test_data]
    print('classified: ',classified)
    print (' ')
    
    counts = Counter((is_cs, cs_probability > 0.5) # (actual, predicted)
                     for _, is_cs, cs_probability in classified)

    print('counts: ',counts)
    print (' ')

if __name__ == "__main__":
    train_and_test_model(r"C:\Users\Nachammai\Desktop\Data Mining\Project 1\docs\*.*")

