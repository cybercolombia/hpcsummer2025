import numpy as np
import pandas as pd
from time import perf_counter
import os

import lc_functions as lcf


if __name__ == '__main__':
    path = os.getcwd()
    # Load Data----------------------------------------
    data = pd.read_csv(path+"/data/train.txt",header=None)
    data.columns = ['feature 1', 'feature 2', 'class']
    data.head()
    
    # Create matrix X and convert to array-------------
    X = data.iloc[:,0:2].to_numpy()
    b = np.ones((X.shape[0],1))
    X = np.hstack((b,X))
    #Create the y one-hot encoding---------------------
    yl = data.iloc[:,2].to_numpy()
    y = np.array([[1 if yl[j]==i else 0 for i in [0,1,2]] for j in range(len(yl))])
    X.shape, y.shape

    lamb = 0.1; # Regularization parameter
    alpha = 0.005; # Convergence parameter
    num_iters = 100000;
    
    #Run gradient descent------------------------------
    Th, ep, hist, t = lcf.grad_desc(X,y,lamb,alpha,num_iters,100)
    
    #Save model and data-------------------------------
    with open(path+"/data/model.dat","w") as model_f:
        for i in range(Th.shape[0]):
            for j in range(Th.shape[1]):
                model_f.write("{0} ".format(Th[i,j]))
            model_f.write("\n")
    
    with open(path+"/data/convergence.dat","w") as cdata:
        for i in range(len(ep)):
            cdata.write("{0} {1}\n".format(ep[i],hist[i]))
    
    print("Matmul time: {0}s".format(t))
    