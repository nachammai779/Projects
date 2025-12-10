import matplotlib.pyplot as plt 
import pylab as pl
import numpy as np
import random

random.seed(1984)
x = []
y = []
for i in range(12):
    randx=random.uniform(-2,10)
    x.append(randx)
    y.append(np.square(randx)+10)
print('X: ', x)
print('Y: ', y)
