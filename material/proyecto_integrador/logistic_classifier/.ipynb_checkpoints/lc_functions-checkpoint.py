import numpy as np
import npmatmul

# Logistic function------------------------------------
def logistic(X, Theta):
    dot = npmatmul.matrix_multiply_omp(X, Theta)
    return 1.0 / (1 + np.exp(-dot))

#Cost function-----------------------------------------
def J(X, y, Theta, lamb):
    A = logistic(X,Theta)
    ones = np.ones((X.shape[0],3))
    A1 = ones - A
    logA = np.log(A)
    logA1 = np.log(A1)
    y1 = ones - y
    
    dot1 = np.sum(np.multiply(y,logA))
    dot2 = np.sum(np.multiply(y1,logA1))
    fact = lamb / (2*y.shape[0])
    thetac = np.copy(Theta)
    thetac[:,0] = 0
    reg = np.sum(fact*thetac**2)
    
    return -(dot1 + dot2)/y.shape[0] + reg

#Gradient of the cost function------------------------
def grad_J(X, y, Theta, lamb):
    A = logistic(X,Theta)
    Ay = A - y
    fact = lamb / y.shape[0]
    thetac = np.copy(Theta)
    thetac[:,0] = 0
    reg = fact*thetac
    
    return npmatmul.matrix_multiply_omp(X.T,Ay)/y.shape[0] + reg

# Gradient descent algorithm---------------------------
def grad_desc(X, y, lamb, alpha, epochs, iprint):
    epoch = []
    history = []
    #Initialize weight matrix
    Theta = 0.1*np.ones((X.shape[1],3))
    for e in range(epochs):
        Theta = Theta - alpha*grad_J(X,y,Theta,lamb)
        if e%iprint == 0:
            epoch.append(e)
            history.append(J(X,y,Theta,lamb))
    return Theta, epoch, history

#Function to predict all the samples in the training set with the trained model
def logi(z):
    return 1.0 / (1 + np.exp(-z))

def predictOneVsAll(X, Theta):
    M = logi(npmatmul.matrix_multiply_omp(X,Theta))
    p = np.array([[np.where(row == np.max(row))[0][0]] for row in M])
    return p

#The boundary is found when logistic(theta_min, x) = 0.5
# which is the same as x . theta_min = 0
def decisionBoundary(theta, xin, xfin, N):
    Dx = (xfin-xin) / N
    a = theta[1] / theta[2]
    b = theta[0] / theta[2]
    boundary = []
    for x in np.arange(xin, xfin+1, Dx):
        boundary.append([x, -a*x-b])
    return pd.DataFrame(boundary, columns=['x1', 'x2'])
