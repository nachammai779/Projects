import math

class Stump:
    def __init__(self):
        self.gtLabel = None
        self.ltLabel = None
        self.splitThreshold = None
        self.splitFeature = None

    def classify(self, point):
        if point[self.splitFeature] >= self.splitThreshold:
            return self.gtLabel
        else:
            return self.ltLabel

    def __call__(self, point):
        return self.classify(point)


def dataToDistribution(data):
    ''' Turn a dataset which has n possible classification labels into a
        probability distribution with n entries. '''
    allLabels = [label for (point, label) in data]
    numEntries = len(allLabels)
    possibleLabels = set(allLabels)

    return [float(allLabels.count(aLabel)) / numEntries for aLabel in possibleLabels]


def entropy(dist):
    ''' Compute the entropy of the given probability distribution. '''
    return -sum([p * math.log(p, 2) for p in dist])


def gain(data, index, threshold):
    entropyGain = entropy(dataToDistribution(data))

    dataSubsets = [
        [(point, label) for (point, label) in data if point[index] >= threshold],
        [(point, label) for (point, label) in data if point[index] < threshold]]

    for dataSubset in dataSubsets:
        entropyGain -= entropy(dataToDistribution(dataSubset))

    return entropyGain


def majorityVote(data):
    ''' Compute the majority of the class labels in the given data set. '''
    labels = [label for (pt, label) in data]
    try:
        return max(set(labels), key=labels.count)
    except:
        return -1


def bestThreshold(data, index):
    thresholds = [point[index] for (point, label) in data]
    return max(thresholds, key=lambda t: gain(data, index, t))


def buildDecisionStump(drawExample):
    data = [drawExample() for _ in range(10)]
    print('Bagging Data: ', data)

    bestThresholds = [(i, bestThreshold(data, i)) for i in range(len(data[0][0]))]
    feature, thresh = max(bestThresholds, key=lambda z: gain(data, z[0], z[1]))

    stump = Stump()
    stump.splitFeature = feature
    stump.splitThreshold = thresh
    stump.gtLabel = majorityVote([x for x in data if x[0][feature] >= thresh])
    stump.ltLabel = majorityVote([x for x in data if x[0][feature] < thresh])
    print('stump.splitThreshold: ', stump.splitThreshold)
    print('stump.gtLabel:', stump.gtLabel)
    print('stump.ltLabel:', stump.ltLabel)
    return stump


if __name__ == "__main__":
    import random

    random.seed(1984)
    data = [([0.5], -1), ([3.0], -1), ([4.5], 1), ([4.6], 1), ([4.9], 1),
            ([5.2], -1), ([5.3], -1), ([5.5], 1), ([7.0], -1), ([9.5], -1)]
    print('data in main:', data)
    s = buildDecisionStump(lambda: random.choice(data))
