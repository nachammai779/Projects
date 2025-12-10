import matplotlib.pyplot as plt 
import pylab as pl
import numpy as np
import random

def estimate_coefficients(x,y):
    #size of the array - in this case it will be 12
    n=np.size(x)
    #mean of x and y
    meanx = np.mean(x)
    meany = np.mean(y)
    #calculating cross validation and deviation about x    
    SSxy = np.sum((x-meanx)*(y-meany))
    SSxx = np.sum(np.square(x-meanx))
    #calculating regression coefficients
    b1 = SSxy/SSxx
    b0 = meany - b1*meanx
    return(b0,b1)

def plot_regression_line(x,y,b):
    #predicted response vector
    ypred = b[0] + (b[1]*x)
    #plotting the regression line
    plt.plot(x,ypred,color='g')



#initial data set
x = np.array([2.3623402702636973, 3.93046818183036, 2.0753210460616236, 0.597853708469624, 8.351827320929244, 8.191995305455864, 2.6294249061387216, 0.9286897816437047, 5.957351530430834, -1.3967469478286905, 8.700246135266521, 4.314283811846619])
y = np.array([15.580651552509558, 25.448580128380854, 14.306957444226311, 10.357429056730883, 79.75301959862016, 77.10878708461091, 16.913875337022624, 10.862464710529432, 45.4900372571266, 11.950902036268763, 85.69428281422005, 28.613044809161792])

#plotting the 12 points in the array
plt.scatter(x,y,color='b',marker='o',s=30)

b = estimate_coefficients(x,y)
print("Estimated coefficients b0, b1: ",b)

plot_regression_line(x,y,b)

#show graph
plt.show()
