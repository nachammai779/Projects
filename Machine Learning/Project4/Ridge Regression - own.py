from __future__ import division
import pandas as pd
import numpy as np
import random

global df
global alpha
global theta
global X
global y
global lmbda
global delta
global train
global test

#Function to compute J_theta (or the cost function)
def costFunc(X,y,theta,length):
    global lmbda
    
    sq_diff = np.sum((np.sum(np.array(X) * theta) - np.array(y))**2)
    cost = (1/(2*(length))) * (sq_diff + (lmbda * np.sum(theta[1:]**2)))
    return cost

#Function to run batch gradient descent algorithm to optimize the cost fuction of the problem.
def gradientDesc(X,y,trainlen,lmbda,alpha,n_iter):
    global theta
    
    costs = list()
    for i in range(0, n_iter):
        theta[0] = theta[0] - (alpha/trainlen) * np.sum((np.sum(np.array(X) * theta) - np.array(y)))
        for j in range(1,len(theta)):
            theta[j] = theta[j]*(1 - ((alpha*lmbda)/trainlen)) - (alpha/trainlen) * (np.sum((np.sum(np.array(X) * theta) - np.array(y)) * np.array(X[j])))
        costs.append(costFunc(X, y, theta, trainlen))
    return costs

def main(A=None):
    global alpha
    global df
    global theta
    global X
    global y
    global lmbda
    global delta
    global train
    global test
    
    lmbda = 1
    alpha = 0.005
    n_iter = 1500

    #initial data set
    
    trainX = np.array([2.3623402702636973, 3.93046818183036, 2.0753210460616236, 0.597853708469624, 8.351827320929244, 8.191995305455864, 2.6294249061387216, 0.9286897816437047, 5.957351530430834,])
    trainY = np.array([15.580651552509558, 25.448580128380854, 14.306957444226311, 10.357429056730883, 79.75301959862016, 77.10878708461091, 16.913875337022624, 10.862464710529432, 45.4900372571266])
    testX = np.array([-1.3967469478286905, 8.700246135266521, 4.314283811846619])
    testY = np.array([11.950902036268763, 85.69428281422005, 28.613044809161792])
    
    theta = [0.0] * (len(list(trainX))) #Theta or parameter vector initialized with 1
    theta = np.array(theta)

    print('before theta:',theta)
    #To train on train data set
    costs = gradientDesc(trainX,trainY,9,lmbda,alpha,n_iter)
    print('after theta:',theta)
    
    test_x = testX
    test_y = testY
    
    #To test model on test set
    predictions = test_x * np.mean(theta)
    residuals = np.sum(np.array(test_x) * np.mean(theta)) - test_y
    residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
    cost = sum(residuals)/len(residuals)
    
    return cost
    #Normal Equation method comparison
    #theta_num  = np.dot(np.dot((np.linalg.inv(np.dot(np.matrix.transpose(np.array(X)),np.array(X)))), np.matrix.transpose(np.array(X))), np.array(y))
    #cost = costFunc(X,y,theta_num)

if __name__ == '__main__':
    cost = main()
    print ('Mean Absolute Percentage Error of our Model on test set:',cost,'%')

#Now we use the built-in ridge regression module in sci-kit learn on our data set to compare performance with the one we built above.
from sklearn.linear_model import Ridge

clf = Ridge(alpha=1, solver='sag', max_iter=1500) #We use the stochastic averaged gradient descent algorihtm since batch GD isn't available
trainX = np.array([2.3623402702636973, 3.93046818183036, 2.0753210460616236, 0.597853708469624, 8.351827320929244, 8.191995305455864, 2.6294249061387216, 0.9286897816437047, 5.957351530430834,])
trainY = np.array([15.580651552509558, 25.448580128380854, 14.306957444226311, 10.357429056730883, 79.75301959862016, 77.10878708461091, 16.913875337022624, 10.862464710529432, 45.4900372571266])
model = clf.fit(trainX, trainY)

testX = np.array([-1.3967469478286905, 8.700246135266521, 4.314283811846619])
testY = np.array([11.950902036268763, 85.69428281422005, 28.613044809161792])
pred = model.predict(test_x)
residuals = pred - test_y
residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
cost = sum(residuals)/len(residuals)
print ('Mean Absolute Percentage Error of built-in model on test set:',cost,'%')

