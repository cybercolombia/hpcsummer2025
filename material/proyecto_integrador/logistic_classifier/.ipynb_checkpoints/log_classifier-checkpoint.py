import numpy as np
import pandas as pd

import lc_functions as lcf


if __name__ == '__main__':
    lamb = 0.1; # Regularization parameter
    alpha = 0.005; # Convergence parameter
    num_iters = 100000;

    # Load Data----------------------------------------
    data = pd.read_csv("train.txt",header=None)
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

    #Run gradient descent------------------------------
    Th, ep, hist = lcf.grad_desc(X,y,lamb,alpha,num_iters,100)

    with open("model.dat","w") as model_f:
        for i in range(Th.shape[0]):
            for j in range(Th.shape[1]):
                model_f.write("{0} ".format(Th[i,j]))
            model_f.write("\n")
    
    with open("convergence.dat","w") as cdata:
        for i in range(len(ep)):
            cdata.write("{0} {1}\n".format(ep[i],hist[i]))
    
    # Test accuracy------------------------------------

    # Read test data
    test_data = pd.read_csv("test.txt",header=None)
    test_data.columns = ['feature 1', 'feature 2', 'class']
    test_data.head()

    #Create matrix Xtest and convert to array
    Xt = test_data.iloc[:,0:2].to_numpy()
    bt = np.ones((Xt.shape[0],1))
    Xt = np.hstack((bt,Xt))
    #Create the ytest vector
    yt = test_data.iloc[:,2].to_numpy()[np.newaxis].T #create a new axis to convert a 1d array
    Xt.shape, yt.shape

    # Predict test data---------------------------------
    pred = lcf.predictOneVsAll(Xt,Th)

    print("Training set accuracy: {0:.3f}".format(np.mean(pred==yt)))
    