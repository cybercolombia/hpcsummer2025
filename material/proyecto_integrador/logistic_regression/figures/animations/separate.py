import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim
from matplotlib import rc
rc('text',usetex=True)


if __name__ == '__main__':
    writer = anim.FFMpegWriter(fps=8)

    #fix_seed for reproducibility
    np.random.seed(1968001)
    
    x1 = np.random.uniform(100,180,10)
    x2 = np.random.uniform(160,250,10)

    fig, ax = plt.subplots()
    ax.set_xlim(100,250)
    ax.set_ylim(-0.5,1.5)
    ax.set_yticks([0,1])
    ax.set_xlabel(r"$W$")
    ax.set_ylabel("category")
    
    Plt1, = plt.plot([],[],linestyle='',marker='o',color='b')
    Plt2, = plt.plot([],[],linestyle='',marker='o',color='b')
    
    y1List = []
    y2List = []
    
    with writer.saving(fig, "separate.mp4", dpi=100):
        for i in range(5):
            y1 = [0.5 for i in range(len(x1))]
            y2 = [0.5 for i in range(len(x2))]
            
            Plt1.set_data(x1,y1)
            Plt2.set_data(x2,y2)
            writer.grab_frame()

        for r in np.arange(0,0.55,0.05):
            y1 = [0.5-r for i in range(len(x1))]
            y2 = [0.5+r for i in range(len(x2))]
            
            Plt1.set_data(x1,y1)
            Plt2.set_data(x2,y2)

            writer.grab_frame()

        for i in range(5):
            writer.grab_frame()
