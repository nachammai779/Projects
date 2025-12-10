#Python Code to numerically calculate gbar(x), E[Eout], bias and var
import matplotlib.pyplot as plt
import pylab as pl
import numpy as np
import random

random.seed(1984)
x = np.arange(-1, 1, 0.001)
y = np.square(x)
ahat = []
bhat = []
for i in range(50000):
    randx1 = random.uniform(-1, 1)
randx2 = random.uniform(-1, 1)
randy1 = randx1 * randx1
randy2 = randx2 * randx2
a = (randy1 - randy2) / (randx1 - randx2)
ahat.append(a)
b = ((randy2 * randx1) - (randy1 * randx2)) / (randx1 - randx2)
bhat.append(b)
if i % 1 == 0:
    plt.plot(x, ((x * a) + b), c='b')
plt.plot(x, y, c='r')
abar = np.mean(ahat)
bbar = np.mean(bhat)
diffahatabar = []
diffbhatbbar = []
for z in range(50000):
    diffahatabar.append(ahat[z] - abar)
diffbhatbbar.append(bhat[z] - bbar)
mdiffahatabar = np.mean(diffahatabar)
mdiffbhatbbar = np.mean(diffbhatbbar)
msqdiffahatabar = np.mean(np.square(diffahatabar))
msqdiffbhatbbar = np.mean(np.square(diffbhatbbar))
mmuldiff = np.mean(mdiffahatabar * mdiffbhatbbar)
xnew = []
xnewsq = []
xnewcu = []
xnewqu = []
biasx = []
varx = []
gbarx = []
for j in range(100000):
    randxnew = random.uniform(-1, 1)
xnew.append(randxnew)
xnewsq.append(np.square(randxnew))
xnewcu.append(np.power(randxnew, 3))
xnewqu.append(np.power(randxnew, 4))
biasx.append(np.square((abar * randxnew) + bbar - (np.square(randxnew))))
varx.append((msqdiffahatabar * np.square(randxnew)) + (2 * mmuldiff * randxnew)
            + (msqdiffbhatbbar))
gbarx.append((abar * randxnew) + bbar)
xnewqumean = np.mean(xnewqu)
xnewcumean = np.mean(xnewcu)
xnewsqmean = np.mean(xnewsq)
xnewmean = np.mean(xnew)
Eoutx = []
for k in range(50000):
    Eoutxk = xnewqumean - (2 * ahat[k] * xnewcumean) + ((np.square(ahat[k]) -
                                                         (2 * bhat[k])) * xnewsqmean) + (
                         2 * ahat[k] * bhat[k] * xnewmean) + np.square(bhat[k])
Eoutx.append(Eoutxk)
EXPEout = np.mean(Eoutx)
print('EXPEout: ', EXPEout)
bias = np.mean(biasx)
print('Bias: ', bias)
var = np.mean(varx)
print('Variance: ', var)
gbar = np.mean(gbarx)
print('GBar: ', gbar)
plt.show()


