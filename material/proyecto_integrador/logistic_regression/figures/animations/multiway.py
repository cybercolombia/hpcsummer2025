import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim
from matplotlib import rc
rc('text',usetex=True)


if __name__ == '__main__':
    writer = anim.FFMpegWriter(fps=8)

    #fix_seed for reproducibility
    np.random.seed(1968001)

    x1 = np.array([0.5,0.7,0.8,1.0])
    y1 = np.array([0.3,0.5,0.2,0.2])
    
    x2 = np.array([0.4,0.8,0.6])
    y2 = np.array([0.9,1.0,0.8])
    
    x3 = np.array([1.1,1.1,1.2,1.3,1.5])
    y3 = np.array([0.6,0.9,0.8,1.1,0.7])

    fig, ax = plt.subplots()
    ax.set_xlim(0.2,1.6)
    ax.set_ylim(0.1,1.2)
    #ax.set_yticks([0,1])
    ax.set_xlabel(r"feature 1")
    ax.set_ylabel(r"feature 2")

    Pnt1, = ax.plot(x1,y1,linestyle='',marker='s',markersize=12)
    Pnt2, = ax.plot(x2,y2,linestyle='',marker='o',markersize=12)
    Pnt3, = ax.plot(x3,y3,linestyle='',marker='^',markersize=12)

    xline = np.linspace(0,1.5,10)
    line1 = np.array([-0.5*i+0.93 for i in xline])
    line2 = np.array([1.2*i-0.12 for i in xline])
    line3 = np.array([-6*i+6.7 for i in xline])
    
    with writer.saving(fig, "multiway.mp4", dpi=100):
        for i in range(15):
            writer.grab_frame()

        for i in range(15):
            Pnt1.set_color('b')
            Pnt2.set_color('b')
            Pnt3.set_color('b')
            
            writer.grab_frame()

        for i in range(8):
            if i%2 == 0:
                Pnt1.set_color('b')
            else:
                Pnt1.set_color('r')
            Pnt2.set_color('b')
            Pnt3.set_color('b')
            
            writer.grab_frame()
            writer.grab_frame()
        ax.plot(xline,line1,color='k')

        for i in range(15):
            writer.grab_frame()

        for i in range(8):
            Pnt1.set_color('b')
            if i%2 == 0:
                Pnt2.set_color('b')
            else:
                Pnt2.set_color('r')
            Pnt3.set_color('b')
            
            writer.grab_frame()
            writer.grab_frame()
        ax.plot(xline,line2,color='k')

        for i in range(15):
            writer.grab_frame()

        for i in range(8):
            Pnt1.set_color('b')
            Pnt2.set_color('b')
            if i%2 == 0:
                Pnt3.set_color('b')
            else:
                Pnt3.set_color('r')
            
            writer.grab_frame()
            writer.grab_frame()
        ax.plot(xline,line3,color='k')

        for i in range(15):
            writer.grab_frame()
