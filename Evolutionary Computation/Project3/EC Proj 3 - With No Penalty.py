import random
# 0-1 Knapsack Problem solved using Genetic Algorithms

# Random seed to get the same result in each run
random.seed(1984)

# Item definition
class Item(object):
    def __init__(self, v, w):
        self.value = v # Item's value. You want to maximize that!
        self.weight = w # Item's weight. The sum of all items should be <= CAPACITY

# Generates 0-10 random items with value = 0-10 and weight = 0-10
ITEMS = [Item(random.randint(0,20),random.randint(0,20)) for x in range (0,20)]

# Capacity of the knapsack randomized according to number of items
CAPACITY = 10*len(ITEMS)
#CAPACITY = 1000
#CAPACITY = 50

# Size of initial population filled with some permutation of 0s and 1s
POP_SIZE = 20

# Maximum number of generations the algorithm will run
GEN_MAX = 50

# Start initial population with only zeros? If not, random permutation of 0s and 1s will be given
START_POP_WITH_ZEROES = False

#fitness(target) will return the fitness value of permutation named "target".
def fitness(target):
    total_value = 0
    total_weight = 0
    index = 0
    for i in target:        
        if index >= len(ITEMS):
            break
        if (i == 1):
            total_value += ITEMS[index].value
            total_weight += ITEMS[index].weight
        index += 1
    
    if total_weight > CAPACITY:
        # Need to be Ignored/Penalized
        return 0
    else:
        # OK to be considered as a solution
        return total_value

def spawn_starting_population(amount):
    return [spawn_individual() for x in range (0,amount)]

def spawn_individual():
    if START_POP_WITH_ZEROES:
        return [random.randint(0,0) for x in range (0,len(ITEMS))]
    else:
        return [random.randint(0,1) for x in range (0,len(ITEMS))]

# mutate - Changes a random element of the permutation array from 0 -> 1 or from 1 -> 0.
def mutate(target):
    r = random.randint(0,len(target)-1)
    if target[r] == 1:
        target[r] = 0
    else:
        target[r] = 1

def evolve_population(pop):
    parent_eligibility = 0.2
    mutation_chance = 0.08
    parent_lottery = 0.05
    #print('population in evolve_population: ', pop)
    parent_length = int(parent_eligibility*len(pop))
    parents = pop[:parent_length]
    nonparents = pop[parent_length:]
    #print('parent_length: ', parent_length)
    #print('parents: ', parents)
    #print('nonparents: ', nonparents)

    for np in nonparents:
        if parent_lottery > random.random():
            parents.append(np)
    #print('parents after lottery: ', parents)

    for p in parents:
        if mutation_chance > random.random():
            mutate(p)
    #print('parents after mutation: ', parents)

    children = []
    desired_length = len(pop) - len(parents)
    #print('desired_length: ', desired_length)
    while len(children) < desired_length :
        p1 = pop[random.randint(0,len(parents)-1)]
        p2 = pop[random.randint(0,len(parents)-1)]        
        half = int(len(p1)/2)
        #print('p1: ', p1)
        #print('p2: ', p2)
        #print('half: ',half)
        child = p1[:half] + p2[half:] # from start to half from p1, from half to end from p2
        #print('child: ', child)
        if mutation_chance > random.random():
            mutate(child)
        #print('child after mutate: ', child)
        children.append(child)

    #print('children: ', children)
    parents.extend(children)
    #print('parents after evaluation: ', parents)

    return parents

def main():
    population = spawn_starting_population(POP_SIZE)
    # Display Items, Capacity and Initial Population 
    for x in range (0,20):
        print('X %d Items.weight %d with Items.value %d ' % (x,ITEMS[x].weight,ITEMS[x].value))
    print('Capacity: ',CAPACITY)
    print('Initial Population: ', population)
    
    generation = 1
    for g in range(0,GEN_MAX):
        print(' ')
        print("Generation %d with pop size %d with best fit below:" % (generation,len(population)))
        population = sorted(population, key=lambda x: fitness(x), reverse=True)
        print('%s, fit: %s' % (population[0], fitness(population[0])))
        print(' ')
        population = evolve_population(population)
        generation += 1

if __name__ == "__main__":
    main()
