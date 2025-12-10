from __future__ import division
import numpy as np
import ga_tools as ga_tools

U = np.random.uniform
N = np.random.normal


def pos_swap(chromosomes, prob):
    """
    :param chromosomes:
    :param prob:
    :return:
    """
    ga_tools._check(len(chromosomes) > 0, "The population cannot be an empty matrix")

    # Convert to 2D matrix if it is a 1D array
    if len(chromosomes.shape) == 1:
        chromosomes = np.array([chromosomes])

    ga_tools._check(len(chromosomes.shape) == 2, "The chromosomes can only be a 1D array or 2D matrix")

    for row in chromosomes:
        if U(0, 1) < prob:
            v1 = np.random.randint(len(row))
            v2 = np.random.randint(len(row))
            while v1 == v2:
                v2 = np.random.randint(len(row))
            row[v1], row[v2] = row[v2], row[v1]
    return chromosomes


def uniform(chromosomes, prob, upper, lower):
    """
    :param chromosomes:
    :param prob:
    :param upper:
    :param lower:
    :return:
    """
    ga_tools.check(len(chromosomes) > 0, "The population cannot be an empty matrix")

    # Convert to 2D matrix if it is a 1D array
    if len(chromosomes.shape) == 1:
        chromosomes = np.array([chromosomes])
    if len(upper.shape) == 1:
        upper = np.array([upper])
    if len(lower.shape) == 1:
        lower = np.array([lower])

    ga_tools.check(len(chromosomes.shape) == 2, "The chromosomes can only be a 1D array or 2D matrix")

    # Create matrix of booleans that determine wether to mutate or not
    to_mutate = U(0, 1, chromosomes.shape) < prob

    # Iterate over each value of the chromosomes
    for i in range(len(chromosomes)):
        for j in range(len(chromosomes[i])):
            # Check if we need to mutate
            if to_mutate[i, j]:
                # pre-compute the value after the mutation
                aux = chromosomes[i, j] + (U(0, 1) - 0.5) * 0.5 * (upper[i, j] - lower[i, j])

                # check that the pre-computed mutation is between the lower and upper bounds.
                # repeat the process if it is not
                while (lower[i, j] > aux or upper[i, j] < aux):
                    aux = chromosomes[i, j] + (U(0, 1) - 0.5) * 0.5 * (upper[i, j] - lower[i, j])

                # once the mutation is between the problem bounds assign it to the chromosomes value
                chromosomes[i, j] = aux

    # return the new generated chromosomes
    return chromosomes


def non_uniform(chromosomes, prob, upper, lower, t, tmax, b=5.):
    """
    :param chromosomes:
    :param prob:
    :param upper:
    :param lower:
    :param t:
    :param tmax:
    :param b:
    :return:
    """
    ga_tools.check(len(chromosomes) > 0, "The population cannot be an empty matrix")

    # Convert to 2D matrix if it is a 1D array
    if len(chromosomes.shape) == 1:
        chromosomes = np.array([chromosomes])
    if len(upper.shape) == 1:
        upper = np.array([upper])
    if len(lower.shape) == 1:
        lower = np.array([lower])

    ga_tools.check(len(chromosomes.shape) == 2, "The chromosomes can only be a 1D array or 2D matrix")

    # Create matrix of booleans that determine wether to mutate or not
    to_mutate = U(0, 1, chromosomes.shape) < prob

    def f(value, i, j):
        """
        :param value:
        :param i:
        :param j:
        :return:
        """
        tau = upper[i, j] - lower[i, j] if U(0, 1) > 0.5 else lower[i, j] - upper[i, j]
        snd_part = (1. - U(0, 1) ** ((1. - t / tmax) ** b))
        return value + tau * snd_part

    for i in range(len(chromosomes)):
        for j in range(len(chromosomes[i])):
            if to_mutate[i, j]:
                aux = f(chromosomes[i, j], i, j)
                while (lower[i, j] > aux or upper[i, j] < aux):
                    aux = f(chromosomes[i, j], i, j)
                chromosomes[i, j] = aux

    return chromosomes


def gaussian(chromosomes, prob, lower, upper, sigma):
    """
    :param chromosomes:
    :param prob:
    :param lower:
    :param upper:
    :param sigma:
    :return:
    """
    ga_tools.check(len(chromosomes) > 0, "The chromosomes population cannot be an empty matrix")

    # Create matrix of booleans that determine wether to mutate or not
    to_mutate = U(0, 1, chromosomes.shape) < prob

    # Create the learning rate parameter
    tau = 1 / np.sqrt(len(chromosomes))

    for i in range(len(chromosomes)):
        for j in range(len(chromosomes[i])):
            if to_mutate[i, j]:
                sigma = sigma * np.exp(tau * N(0, 1))
                chromosomes[i, j] = chromosomes[i, j] + sigma * N(0, 1)

    # Verify that the chromosomes value is not out of bounds
    chromosomes = np.maximum.reduce([chromosomes, lower])
    chromosomes = np.minimum.reduce([chromosomes, upper])

    return chromosomes, sigma


def gga(s, alpha, delta, control_alpha, control_s, prob, prob_alpha, upper_s, lower_s):
    """
    :param s:
    :param alpha:
    :param prob:
    :param prob_alpha:
    :return:
    """
    ga_tools.check(len(s) > 0, "S cannot be an empty array")
    ga_tools.check(len(alpha) > 0, "Alpha cannot be an empty array")
    ga_tools.check(s.shape == alpha.shape, "alpha and S must have the same size")

    to_mutate = U(0, 1, alpha.shape) < prob
    alpha_mutation = U(0, 1, alpha.shape) < prob_alpha

    for i in range(len(s)):
        for j in range(len(s[i])):

            # Check if the gene has to be mutated
            if to_mutate[i, j]:

                # Check which kind of mutation to apply
                if alpha_mutation[i, j]:
                    alpha[i, j] += U(-delta[i, j] * control_alpha, delta[i, j] * control_alpha)
                    if alpha[i, j] < 0:
                        alpha[i, j] += delta[i, j]
                        s[i, j] -= 1
                    elif alpha[i, j] > delta[i, j]:
                        alpha[i, j] -= delta[i, j]
                        s[i, j] += 1
                else:
                    s[i, j] += ga_tools.geometric(control_s) - ga_tools.geometric(control_s)

    # Check that s is inside the upper bounds of the discretized space
    mask = s > upper_s
    s[mask] = upper_s[mask]

    # Check that s is inside the lower bounds of the discretized space
    mask = s < lower_s
    s[mask] = lower_s[mask]

    return s, alpha