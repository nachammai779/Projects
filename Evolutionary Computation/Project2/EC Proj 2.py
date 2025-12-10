
import numpy as np
import random

global Y
global Ynew
global yfitness
global yfitness_low
global fitness_low
global yfitness_sum
global yfitness_relative
global yfitness_cumulative
global eval_counter
global cross_type
global cross_type_aplha_val
global mut_type
global sel_type
global sel_type_tau_val
global sel_rank_max
global sel_rank_min

TOTAL_RUN = 30
FX_ONE_SET_SIZE = 3
FX_ONE_GEN_SIZE = 10
FX_MAX_GEN = 50
CROSS_RATE = 0.8
MUT_RATE = 0.1
LOWER_BOUND = -1
UPPER_BOUND = 5
eval_counter = 0

## Function to return the fitness value for an x[3].
## Hardcoded this to size 3 only.
## If needed we need to expand this based on FX_ONE_SET_SIZE.
def f(x):
    global eval_counter
    eval_counter = eval_counter + 1
    return np.square(x[0]) + np.square(x[1]) + np.square(x[2])

## Function for the first generation of 10*3 array in Y[]
def firsttime_generate():
    global Y

    for j in range(FX_ONE_GEN_SIZE):
        y=[]
        for i in range(FX_ONE_SET_SIZE):
            y.append(np.random.uniform(LOWER_BOUND,UPPER_BOUND))
        Y.append(y)

## Select 
def select():
    global Y
    global Ynew
    global yfitness
    global yfitness_low
    global fitness_low
    global yfitness_sum
    global yfitness_relative
    global yfitness_cumulative
    global sel_type
    global sel_type_tau_val
    global sel_rank_max
    global sel_rank_min
    
    fitness_yj = 0
    Ynew = []        
    yfitness = []
    yrankfitness = []
    yfitness_relative = []
    yfitness_cumulative = []
    yfitness_sum = 0
    yrankfitsum = 0
    
    for j in range(FX_ONE_GEN_SIZE):
        fitness_yj = f(Y[j])
        yfitness.append(fitness_yj)
        yfitness_sum = yfitness_sum + fitness_yj
        if (fitness_yj < fitness_low):
            fitness_low = fitness_yj
            yfitness_low = Y[j]

    if (sel_type == 1): ## Proportional Selection
        for j in range(FX_ONE_GEN_SIZE):
            yfitness_relative.append(yfitness[j]/yfitness_sum)

        yfitness_c = 0
        for j in range(FX_ONE_GEN_SIZE):
            yfitness_c = yfitness_c + yfitness_relative[j]
            yfitness_cumulative.append(yfitness_c)

        for j in range(FX_ONE_GEN_SIZE):
            p=np.random.uniform(0,1)
            if (p < yfitness_cumulative[0]):
                Ynew.append(Y[0])
            else:
                for k in range(FX_ONE_GEN_SIZE-1):
                    if (p >= yfitness_cumulative[k] and p < yfitness_cumulative[k+1]):
                        Ynew.append(Y[k+1])
                        break

    elif (sel_type == 2): ## Ranking Selection
        yfitness.sort(reverse=True)
        Y.sort(key=f,reverse=True)

        for j in range(FX_ONE_GEN_SIZE):
            rankp = (1/FX_ONE_GEN_SIZE) * (sel_rank_min + ((j/(FX_ONE_GEN_SIZE-1)) *(sel_rank_max - sel_rank_min)))
            yrankfitness.append(rankp)
            yrankfitsum = yrankfitsum + rankp

        for j in range(FX_ONE_GEN_SIZE):
            yfitness_relative.append(yrankfitness[j]/yrankfitsum)

        yfitness_c = 0
        for j in range(FX_ONE_GEN_SIZE):
            yfitness_c = yfitness_c + yfitness_relative[j]
            yfitness_cumulative.append(yfitness_c)

        for j in range(FX_ONE_GEN_SIZE):
            p=np.random.uniform(0,1)
            if (p < yfitness_cumulative[0]):
                Ynew.append(Y[0])
            else:
                for k in range(FX_ONE_GEN_SIZE-1):
                    if (p >= yfitness_cumulative[k] and p < yfitness_cumulative[k+1]):
                        Ynew.append(Y[k+1])
                        break

    elif (sel_type == 3): ## Tournament Selection
        for j in range(FX_ONE_GEN_SIZE):
            point1 = np.random.randint((FX_ONE_GEN_SIZE-1))
            point2 = np.random.randint((FX_ONE_GEN_SIZE-1))
            while (point1 == point2):
                point2 = np.random.randint((FX_ONE_GEN_SIZE-1))
            if (yfitness[point1] <= yfitness[point2]):
                Ynew.append(Y[point1])
            else:
                Ynew.append(Y[point2])

    elif (sel_type == 4): ## Truncation Selection
        yfitness.sort()
        Y.sort(key=f)

        for j in range(FX_ONE_GEN_SIZE):
            yfitness_relative.append(yfitness[j]/yfitness_sum)

        yfitness_c = 0
        for j in range(FX_ONE_GEN_SIZE):
            yfitness_c = yfitness_c + yfitness_relative[j]
            yfitness_cumulative.append(yfitness_c)

        for j in range(FX_ONE_GEN_SIZE):
            p=np.random.uniform(0,(sel_type_tau_val/100))
            if (p < yfitness_cumulative[0]):
                Ynew.append(Y[0])
            else:
                for k in range(FX_ONE_GEN_SIZE-1):
                    if (p >= yfitness_cumulative[k] and p < yfitness_cumulative[k+1]):
                        Ynew.append(Y[k+1])
                        break
                    
    else: print('Invalid Selection Type')
    
    Y = Ynew                

## Crossover
def Xover(i,j):
    global Y
    if (FX_ONE_SET_SIZE >1):
        if (FX_ONE_SET_SIZE == 2):
            point = 1
        else:
            point = np.random.randint((FX_ONE_SET_SIZE-1))+1

        tempyi = Y[i]
        tempyj = Y[j]
        Y[i]=tempyi[:point]+tempyj[point:]
        Y[j]=tempyj[:point]+tempyi[point:]

## Arithmetic Crossover
def ArithXover(i,j):
    global Y
    global cross_type_aplha_val
    tempyi = Y[i]
    tempyj = Y[j]
    one_minus_cross_type_aplha_val = 1 - cross_type_aplha_val
    
    for k in range(FX_ONE_SET_SIZE):
        Y[i][k] = (cross_type_aplha_val*tempyi[k]) + (one_minus_cross_type_aplha_val*tempyj[k])
        Y[j][k] = (cross_type_aplha_val*tempyj[k]) + (one_minus_cross_type_aplha_val*tempyi[k])
    
## Crossover
def crossover():
    global Y
    global cross_type
    temp = 0
    i=0
    for j in range(FX_ONE_GEN_SIZE):
        p=np.random.uniform(0,1)
        if (p < CROSS_RATE):
            temp = temp + 1
            if (temp%2==0):
                if (cross_type == 1):
                    Xover(i,j)
                else:
                    ArithXover(i,j)
            else:
                i=j

## Mutate 
def mutate():
    global Y
    global mut_type
    sigma = 1/5
    
    for i in range(FX_ONE_GEN_SIZE):
        for j in range(FX_ONE_SET_SIZE):
            p=np.random.uniform(0,1)
            if (p < MUT_RATE):
                if (mut_type == 1):
                    Y[i][j] = np.random.uniform(LOWER_BOUND,UPPER_BOUND)
                else:
                    for k in range(4):
                        tempYij = Y[i][j] + (sigma * np.random.normal(0,1))
                        if (tempYij >= LOWER_BOUND and tempYij <= UPPER_BOUND):
                            Y[i][j] = tempYij
                            break
                    if (Y[i][j] != tempYij):
                        if (tempYij > UPPER_BOUND):
                            Y[i][j] = UPPER_BOUND
                        else:
                            Y[i][j] = LOWER_BOUND

## Main function
def main(seed_value):
    global Y
    global Ynew
    global yfitness
    global yfitness_low
    global fitness_low
    global yfitness_sum
    global yfitness_relative
    global yfitness_cumulative
    
    Y = []
    Ynew = []
    yfitness = []
    yfitness_low = []
    fitness_low = 75
    yfitness_sum = 0
    yfitness_relative = []
    yfitness_cumulative = []

    np.random.seed(seed_value)
    
    firsttime_generate()

    for gen in range(FX_MAX_GEN):
        select()
        crossover()
        mutate()
        #if ((gen+1)%10==0):
            #print('Gen_Num: ', gen)
            #print('Av_fitness of Cur_gen: ', np.average(yfitness))
            #print('Std_Dev of Av_Fitness of Cur_gen: ', np.std(yfitness))

    return fitness_low, yfitness_low

## 1. Get the required parameters from user
cross_type = float(input("Enter Crossover Type - 1 for Single Point (or) 2 for Arithmetic: "))
if cross_type == 2:
    cross_type_aplha_val = float(input("For Arithmetic Crossover, Enter Alpha value between 0 and 1: "))

mut_type = float(input("Enter Mutation Type - 1 for Uniform-Random (or) 2 for Gaussian: "))
print('Enter Selection Type - 1 for Proportional (or) 2 for Ranking (or) 3 for Tournament (or) 4 for Truncation : ')
sel_type = float(input())
if sel_type == 2:
    sel_rank_max = float(input("For Ranking Selection, Enter Max value between 1 and 2: "))
    sel_rank_min = 2 - sel_rank_max
if sel_type == 4:
    sel_type_tau_val = float(input("For Truncation Selection, Enter Tau percent value between 40 and 60: "))


print('cross_type: ', cross_type)
if cross_type == 2:
    print('cross_type_aplha_val: ', cross_type_aplha_val)
print('mut_type: ', mut_type)
print('sel_type: ', sel_type)
if sel_type == 2:
    print('sel_rank_max: ', sel_rank_max)
    print('sel_rank_min: ', sel_rank_min)
if sel_type == 4:
    print('sel_type_tau_val: ', sel_type_tau_val)

## Running the Main function TOTAL_RUN times
bestfits = []
bestfitoverall = 75
bestfitoverall_vector = []
for totalrun in range(TOTAL_RUN):
    #print(' ')
    #print(' ')
    #print('Run Number: ', totalrun)
    bestfit, bestfit_vector = main(np.square(totalrun))
    #print(' ')
    #print('Best_of_run Solution: ', bestfit)
    #print('Vector of best_of_run: ', bestfit_vector)
    bestfits.append(bestfit)
    if (bestfit < bestfitoverall):
        bestfitoverall = bestfit
        bestfitoverall_vector = bestfit_vector

print(' ')
print(' ')
print('Overall Eval Counter: ', eval_counter)
print('Overall Best_of_run Solution: ', bestfitoverall)
print('Vector of overall best_of_run: ', bestfitoverall_vector)
print('Average of overall best fits: ', np.average(bestfits))
print('Std_Dev of overall best fits: ', np.std(bestfits))


