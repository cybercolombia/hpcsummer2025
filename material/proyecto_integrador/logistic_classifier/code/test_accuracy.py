import numpy as np
import pandas as pd
import os

import lc_functions as lcf

# Test accuracy------------------------------------


if __name__ == '__main__':
    #Load model
    path = os.getcwd()
    Th = []
    with open(path+"/code/data/model.dat","r") as mdata:
        for line in mdata:
            v = list(map(float,line.split()))
            Th.append(v)
    Th = np.asarray(Th)
    
    # Read test data
    test_data = pd.read_csv(path+"/code/data/test.txt",header=None)
    test_data.columns = ['feature 1', 'feature 2', 'class']
    test_data.head()

    #Create matrix Xtest and convert to array
    Xt = test_data.iloc[:,0:2].to_numpy()
    bt = np.ones((Xt.shape[0],1))
    Xt = np.hstack((bt,Xt))
    #Create the ytest vector
    yt = test_data.iloc[:,2].to_numpy()[np.newaxis].T #create a new axis to convert a 1d array

    # Predict test data---------------------------------
    pred = lcf.predictOneVsAll(Xt,Th)

    print("Training set accuracy: {0:.3f}".format(np.mean(pred==yt)))