import numpy as np
import random
import matplotlib.pyplot as plt 
#import pylab as pl

global Y
global Ynew
global yfitness
global yfitness_low
global fitness_low
global yfitness_sum
global yfitness_relative
global yfitness_cumulative

TOTAL_RUN = 30
FX_ONE_SET_SIZE = 3
FX_ONE_GEN_SIZE = 10
FX_MAX_GEN = 50
CROSS_RATE = 0.8
MUT_RATE = 0.1
LOWER_BOUND = -1
UPPER_BOUND = 5

## Function to return the fitness value for a x[3]
## Hardcoded this to size 3 only
## If needed we need to expand this based on FX_ONE_SET_SIZE
def f(x):
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
    fitness_yj = 0
    Ynew = []        
    yfitness = []
    yfitness_relative = []
    yfitness_cumulative = []
    yfitness_sum = 0
    
    for j in range(FX_ONE_GEN_SIZE):
        fitness_yj = f(Y[j])
        yfitness.append(fitness_yj)
        yfitness_sum = yfitness_sum + fitness_yj
        if (fitness_yj < fitness_low):
            fitness_low = fitness_yj
            yfitness_low = Y[j]
    
    for j in range(FX_ONE_GEN_SIZE):
        yfitness_relative.append((f(Y[j]))/yfitness_sum)

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

    #print('Y: ', Y)
    #print('Ynew: ', Ynew)
    Y = Ynew                

## Xover
def Xover(i,j):
    global Y
    if (FX_ONE_SET_SIZE >1):
        if (FX_ONE_SET_SIZE == 2):
            point = 1
        else:
            point = np.random.randint((FX_ONE_SET_SIZE-1))+1

        #print('Before Xover, Y: ', Y)
        #print('i: ', i)
        #print('j: ', j)
        #print('point: ', point)
        tempyi = Y[i]
        tempyj = Y[j]
        Y[i]=tempyi[:point]+tempyj[point:]
        Y[j]=tempyj[:point]+tempyi[point:]
        #print('After Xover, Y: ', Y)
    
## Crossover
def crossover():
    global Y
    temp = 0
    i=0
    #print ('Before crossover, Y: ', Y)
    for j in range(FX_ONE_GEN_SIZE):
        p=np.random.uniform(0,1)
        if (p < CROSS_RATE):
            temp = temp + 1
            if (temp%2==0):
                Xover(i,j)
            else:
                i=j
    #print ('After crossover, Y: ', Y)
    
## Mutate 
def mutate():
    global Y
    #print ('Before Mutation, Y: ', Y)
    for i in range(FX_ONE_GEN_SIZE):
        for j in range(FX_ONE_SET_SIZE):
            p=np.random.uniform(0,1)
            if (p < MUT_RATE):
                Y[i][j] = np.random.uniform(LOWER_BOUND,UPPER_BOUND)
    #print ('After Mutation, Y: ', Y)

## Main function
def main():
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
    
    firsttime_generate()
    print('First Y: ', Y)
    print(' ')

    seed_value = 0
    seed_value = np.random.randint(2000)
    print('Seed Value: ', seed_value)
    np.random.seed(seed_value)
    for gen in range(FX_MAX_GEN):
        select()
        crossover()
        mutate()
        print('Generation Number: ', gen)
        #print('Y: ', Y)
        #print(' ')
        #print('yfitness: ', yfitness)
        print('Average of yfitness...: ', np.average(yfitness))
        print('Standard Deviation of yfitness...: ', np.std(yfitness))
        print('fitness_low: ', fitness_low)
        print('yfitness_low: ', yfitness_low)
        print(' ')

## Running the Main function
for totalrun in range(TOTAL_RUN):
    main()
