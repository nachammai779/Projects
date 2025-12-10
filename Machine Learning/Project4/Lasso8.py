import numpy as np
from numpy.linalg import inv
from matplotlib import pyplot as plt
from scipy import argmax

#y = np.random.randn(5)
#X = np.random.randn(15).reshape(5,3)
X = np.array([2.3623402702636973, 3.93046818183036, 2.0753210460616236, 0.597853708469624, 8.351827320929244, 8.191995305455864, 2.6294249061387216, 0.9286897816437047, 5.957351530430834, -1.3967469478286905, 8.700246135266521, 4.314283811846619])
y = np.array([15.580651552509558, 25.448580128380854, 14.306957444226311, 10.357429056730883, 79.75301959862016, 77.10878708461091, 16.913875337022624, 10.862464710529432, 45.4900372571266, 11.950902036268763, 85.69428281422005, 28.613044809161792])

#center y
y = y-np.mean(y)

# center X
# subtract column means from X 
mean = np.expand_dims(np.mean(X),axis = 0)
mean_stack = mean.copy()
for i in range(X.shape[0]-1):
    mean_stack = np.r_[mean_stack,mean]
X = X - mean_stack

print("mean of y: ", np.mean(y))
print("mean of X: ", np.mean(X, axis = 0))
assert ( abs(np.mean(y)) < 1e-3 )
assert ( any(abs(np.mean(X,axis = 0)) < 1e-3) )

beta =  np.zeros([1,X.shape[1]])  # this will be row binded as iteration goes. 
eps = 0.001
iteration = 4000

# first stage
r = y
inact_set = range(X.shape[1])
for i in range(iteration):
    
    # 1. calculate correation for all dimension and see what is the index with the most correation. 
    cor = X[:,inact_set].T.dot(r)
    jmax = inact_set[argmax(abs(cor[inact_set]))]
    
    # 2. direction to move beta.
    # we are moving in the direction of the < X[jmax], r >
    delta = eps*np.sign(cor[jmax])
    
    # 3. select the beta from the last iteration and add delta to the jmax'th index. 
    b = beta[beta.shape[0]-1,:]
    b[jmax] = b[jmax] + delta
    
    # 4. append b to beta's histroy
    b = np.expand_dims(b, axis = 0)
    beta = np.r_[beta,b]
    
    # 5. update the residual
    r = r - delta*X[:,jmax]

print("Stagewise beta: ", beta[-1,:])
plt.plot(range(iteration+1), beta)
plt.ylabel("beta")
plt.xlabel("iteration")

