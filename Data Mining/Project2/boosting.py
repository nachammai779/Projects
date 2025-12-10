import random
import math

def draw(weights):
    choice = random.uniform(0, sum(weights))
    choiceIndex = 0

    for weight in weights:
        choice -= weight
        if choice <= 0:
            return choiceIndex

        choiceIndex += 1


# normalize a weight distribution
def normalize(weights):
    norm = sum(weights)
    return tuple(m / norm for m in weights)


def sign(x):
    return 1 if x >= 0 else -1
   
# compute the weighted error of a given hypothesis on a weight distribution
def computeError(h, examples, weights=None):
   if weights is None:
      weights = [1.] * len(examples)

   hypothesisResults = [h(x)*y for (x,y) in examples] # +1 if correct, else -1
   return hypothesisResults, sum(w for (z,w) in zip(hypothesisResults, weights) if z < 0)


# boost: [(list, label)], learner, int -> (list -> label)
# where a learner is (() -> (list, label)) -> (list -> label)
# boost the weak learner into a strong learner
def boost(examples, individualLearner, rounds):
   weight_distr = normalize([1.] * len(examples))
   hypotheses = [None] * rounds
   alpha = [0] * rounds
   print('Initial weight distribution: ',weight_distr)
   print('Initial alpha: ', alpha)
   
   for t in range(rounds):
      new_weight_distr = []
      print('Round: ', t)
      def drawExample():
         return examples[draw(weight_distr)]

      hypotheses[t] = individualLearner(drawExample)
      hypothesisResults, error = computeError(hypotheses[t], examples, weight_distr)

      alpha[t] = 0.5 * math.log((1 - error) / error)
      for i in range(len(examples)):
          if (hypothesisResults[i] >= 0):
              new_weight_distr.append(((0.5 * weight_distr[i])/(1-error)))
          else:
              new_weight_distr.append(((0.5 * weight_distr[i])/error))
      weight_distr = new_weight_distr
      
      print('weight distribution: ', weight_distr)
      print('alpha: ', alpha[t])
      print('error %.3f' % error)

   def finalHypothesis(x):
       H = sum(a * h(x) for (a, h) in zip(alpha, hypotheses))
       print('H from finalHypothesis: ',H)
       SH = sign(H)
       print('Sign of H from finalHypothesis: ',SH)
       return SH

   return finalHypothesis

def error(h, data):
   return sum(1 for x,y in data if h(x) != y) / len(data)

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
   print('Training error: %G' % error(h, train))
   print(' ')
   print('Applying Hypothesis on Test Data: ',test)
   print('Test Classification: ', [h(x) for x in test])

if __name__ == "__main__":
   random.seed(1984)
   runMain()
