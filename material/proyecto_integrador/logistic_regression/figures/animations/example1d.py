import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim
from matplotlib import rc
rc('text',usetex=True)


def func(x,m,n):
    return 1. / (1. + np.exp(-n*(x-m)))


if __name__ == '__main__':
    writer = anim.FFMpegWriter(fps=8)

    #fix_seed for reproducibility
    np.random.seed(1968001)

    fig = plt.figure()
    plt.xlim(-5,10)
    plt.ylim(-1,2)
    plt.xlabel("feature")
    plt.ylabel("category")

    x0 = 3
    s = 8.
    d = 0.1
    
    xx = np.random.uniform(-5,10,17)
    xx = np.sort(xx)
    x = np.array([i for i in xx if (i<(x0-d) or (i>x0+d))])
    y = np.array([1 if i > x0 else 0 for i in x])
    #print(x,y)

    m_params = np.linspace(0,3,10)
    n_params = np.linspace(1.,s,10)
    params = zip(m_params,n_params)
    
    xline = np.linspace(-4,9,100)
    ylines = [np.array([func(i,m,n) for i in xline]) for m,n in params]

    lPnt, = plt.plot([],[],linestyle='',marker='o')
    l, = plt.plot([],[],linestyle='-',linewidth=2,color='k')
    llim = plt.axvline(x=x0,linestyle='--',linewidth=2,color='r',alpha=0.0)

    xPntList = []
    yPntList = []
    xLineList = []
    yLineList = []

    with writer.saving(fig, "fit1d.mp4", dpi=100):

        # First show the data points
        for xval,yval in zip(x,y):
            xPntList.append(xval)
            yPntList.append(yval)

            lPnt.set_data(xPntList,yPntList)
            l.set_data(xLineList,yLineList)

            writer.grab_frame()

        # Add the line fit
        for yline in ylines:
            xLineList = xline
            yLineList = yline

            lPnt.set_data(xPntList,yPntList)
            l.set_data(xLineList,yLineList)

            # Double up the frames to slow things down a bit here.
            writer.grab_frame()
            writer.grab_frame()

        # Add the limit
        for i in range(5):
            lPnt.set_data(xPntList,yPntList)
            l.set_data(xLineList,yLineList)

            if(i%2 == 0):
                llim.set_alpha(1.0)
            else:
                llim.set_alpha(0.0)
            
            # Double up the frames to slow things down a bit here.
            writer.grab_frame()
            writer.grab_frame()

        # Make a final pause
        for i in range(10):
            writer.grab_frame()
