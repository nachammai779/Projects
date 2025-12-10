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

#Function to import data
def import_data():
    global df
    
    df = pd.read_excel('data.xlsx')
    df.dropna(inplace=True)
    return

#Function to standardize input features
def stand_norm():
    global df
    
    backup = df
    resp = df['Y']
    df.drop(['Y'], axis=1, inplace=True)
    
    cols = list(df)
    
    stats = df.describe()
    for col in cols:
        def func(x):
            return (x-stats[col][1])/stats[col][2]

        df[col] = df[col].apply(func)
    df['Y'] = resp
    return

#Function to create 2nd degree polynomial features as L2-Regularization isn't really useful with a few number of features.
def feature_map():
    global df
    
    cols = list(df)
    
    backup = df
    resp = df['Y']
    df.drop(['Y'], axis=1, inplace=True)

    nums = range(0,1)
    for i in range(0,18):
        first = random.choice(nums)
        second = random.choice(nums)
        name = cols[first] + "x" + cols[second]
        df[name] = df[cols[first]].multiply(df[cols[second]], axis=0)

    df['Y'] = resp
    return


#Function to compute J_theta (or the cost function)
def costFunc(X,y,theta,length):
    global lmbda
    
    sq_diff = np.sum((np.sum(np.array(X) * theta, axis=1) - np.array(y))**2)
    cost = (1/(2*(length))) * (sq_diff + (lmbda * np.sum(theta[1:]**2)))
    return cost

#Function to run batch gradient descent algorithm to optimize the cost fuction of the problem.
def gradientDesc(X,y,train,lmbda,alpha,n_iter):
    global theta
    
    costs = list()
    for i in range(0, n_iter):
        theta[0] = theta[0] - (alpha/len(train)) * np.sum((np.sum(np.array(X) * theta, axis=1) - np.array(y)))
        for j in range(1,len(theta)):
            theta[j] = theta[j]*(1 - ((alpha*lmbda)/len(train))) - (alpha/len(train)) * (np.sum((np.sum(np.array(X) * theta, axis=1) - np.array(y)) * np.array(X.ix[:,j])))
        costs.append(costFunc(X, y, theta, len(train)))
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
    
    #To import, standardize and map features to achieve final data set.
    import_data()
    stand_norm()
    feature_map()
    
    
#    df = df.iloc[np.random.permutation(len(df)-1)].reset_index(drop=1) # To shuffle the dataset
    df = df.iloc[np.random.permutation(len(df))].reset_index(drop=0) # To shuffle the dataset


#Fold 1
    train = df[0:3]
    test  = df[3:1]
    print('df: ', df)
    print('fold1 train: ', train)
    print('fold1 test: ', test)
    Xn    = pd.Series([1] * len(train)) # Creation feature X_0 which is to hold a value of 1.
    X     = pd.concat([Xn, train.drop(['Y'], axis=1)], axis=1) #Feature set in X
    y     = train['Y'] #Response variable in y
    theta = [0.0] * (len(list(X))) #Theta or parameter vector initialized with 1
    theta = np.array(theta)
    #To train on train data set
    costs = gradientDesc(X,y,train,lmbda,alpha,n_iter)
    Xn    = pd.Series([1] * len(test))
    test.reset_index(drop=1, inplace=True)
    test_x = pd.concat([Xn, test.drop('Y',axis=1)], axis=1)
    test_y = test['Y']
    #To test model on test set
    predictions = test_x * theta
    residuals = np.sum(np.array(test_x) * theta, axis=1) - test_y
    print('residuals 1:', residuals)
    residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
    print('residuals 2:', residuals)
    cost1 = sum(residuals)/len(residuals)

#Fold 2
    train = df[4:3]
    test  = df[7:1]
    Xn    = pd.Series([1] * len(train)) # Creation feature X_0 which is to hold a value of 1.
    X     = pd.concat([Xn, train.drop(['Y'], axis=1)], axis=1) #Feature set in X
    y     = train['Y'] #Response variable in y
    theta = [0.0] * (len(list(X))) #Theta or parameter vector initialized with 1
    theta = np.array(theta)
    #To train on train data set
    costs = gradientDesc(X,y,train,lmbda,alpha,n_iter)
    Xn    = pd.Series([1] * len(test))
    test.reset_index(drop=1, inplace=True)
    test_x = pd.concat([Xn, test.drop('Y',axis=1)], axis=1)
    test_y = test['Y']
    #To test model on test set
    predictions = test_x * theta
    residuals = np.sum(np.array(test_x) * theta, axis=1) - test_y
    residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
    cost2 = sum(residuals)/len(residuals)

#Fold 3
    train = df[8:3]
    test  = df[11:1]
    Xn    = pd.Series([1] * len(train)) # Creation feature X_0 which is to hold a value of 1.
    X     = pd.concat([Xn, train.drop(['Y'], axis=1)], axis=1) #Feature set in X
    y     = train['Y'] #Response variable in y
    theta = [0.0] * (len(list(X))) #Theta or parameter vector initialized with 1
    theta = np.array(theta)
    #To train on train data set
    costs = gradientDesc(X,y,train,lmbda,alpha,n_iter)
    Xn    = pd.Series([1] * len(test))
    test.reset_index(drop=1, inplace=True)
    test_x = pd.concat([Xn, test.drop('Y',axis=1)], axis=1)
    test_y = test['Y']
    #To test model on test set
    predictions = test_x * theta
    residuals = np.sum(np.array(test_x) * theta, axis=1) - test_y
    residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
    cost3 = sum(residuals)/len(residuals)
    
    cost = ((cost1+cost2+cost3)/3)
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
X = np.array(X)
y = np.array(y)
model = clf.fit(X, y)

Xn    = pd.Series([1] * len(test))
test.reset_index(drop=1, inplace=True)
test_x = pd.concat([Xn, test.drop('Y',axis=1)], axis=1)
test_y = test['Y']

pred = model.predict(test_x)
residuals = pred - test_y
residuals = [(abs(a)/b)*100 for a,b in zip(residuals,test_y)]
cost = sum(residuals)/len(residuals)
print ('Mean Absolute Percentage Error of built-in model on test set:',cost,'%')

