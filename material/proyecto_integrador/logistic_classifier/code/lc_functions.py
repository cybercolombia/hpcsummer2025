import numpy as np
import npmatmul

# Logistic function------------------------------------
def logistic(X, Theta):
    dot, t = npmatmul.matrix_multiply(X, Theta)
    #dot, t = npmatmul.matrix_multiply_omp(X, Theta)
    return (1.0 / (1 + np.exp(-dot)), t)

#Cost function-----------------------------------------
def J(X, y, Theta, lamb):
    A, t = logistic(X,Theta)
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
    
    return (-(dot1 + dot2)/y.shape[0] + reg, t)

#Gradient of the cost function------------------------
def grad_J(X, y, Theta, lamb):
    A, t = logistic(X,Theta)
    Ay = A - y
    fact = lamb / y.shape[0]
    thetac = np.copy(Theta)
    thetac[:,0] = 0
    reg = fact*thetac

    mat, t1 = npmatmul.matrix_multiply(X.T,Ay)
    #mat, t1 = npmatmul.matrix_multiply_omp(X.T,Ay)
    return (mat/y.shape[0] + reg, t+t1)

# Gradient descent algorithm---------------------------
def grad_desc(X, y, lamb, alpha, epochs, iprint):
    epoch = []
    history = []
    #Initialize weight matrix
    Theta = 0.1*np.ones((X.shape[1],3))
    t = 0
    for e in range(epochs):
        grad, t1 = grad_J(X,y,Theta,lamb)
        Theta = Theta - alpha*grad
        t2 = 0
        if e%iprint == 0:
            epoch.append(e)
            cost, t2 = J(X,y,Theta,lamb)
            history.append(cost)
        t += t1 + t2
    return Theta, epoch, history, t

#Function to predict all the samples in the training set with the trained model
def logi(z):
    return 1.0 / (1 + np.exp(-z))

def predictOneVsAll(X, Theta):
    mat, t = npmatmul.matrix_multiply(X,Theta)
    #mat, t = npmatmul.matrix_multiply_omp(X,Theta)
    M = logi(mat)
    p = np.array([[np.where(row == np.max(row))[0][0]] for row in M])
    return p


