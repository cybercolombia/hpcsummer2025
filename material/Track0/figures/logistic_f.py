import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rc
rc('text',usetex=True)

def func(x,m,n):
    return 1. / (1. + np.exp(-n*(x-m)))

if __name__ == '__main__':
    n = 2.
    m = 3.
    
    x = np.linspace(-3,6,100)
    y = func(x,m,n)
    l = [0.5 for i in x]
    
    fig = plt.figure()
    plt.xlim(-3,6)
    plt.ylim(-0.5,1.5)
    plt.xlabel(r"$x$",fontsize=15)
    plt.ylabel(r"$y$",fontsize=15)
    plt.tick_params(axis='both',which='major',labelsize=10)
    
    plt.plot(x,y,label=r"$\frac{1}{1+e^{-2(x-3)}}$")
    plt.plot(x,l,linestyle='--',color='r')
    plt.vlines(x=m,ymin=-0.5,ymax=1.5,linestyle='--',color='r')
    plt.legend(loc='upper left',fontsize=20)

    plt.savefig("logistic_f.png")
    #plt.show()
