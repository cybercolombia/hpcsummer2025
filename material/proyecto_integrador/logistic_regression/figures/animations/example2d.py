import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as anim
from matplotlib import cm
from matplotlib import rc
rc('text',usetex=True)

#Line: ax+by+c=0
def func(x0,y0,a,b,c,n):   
    return 1. / (1. + np.exp(-n*(a*x0+b*y0+c)))


if __name__ == '__main__':
    writer = anim.FFMpegWriter(fps=8)

    #fix_seed for reproducibility
    np.random.seed(19680801)

    fig, ax = plt.subplots(subplot_kw=dict(projection='3d'))

    a = 2.
    b = -1.
    c0 = 3.
    s = 0.8
    
    limx = [-20,20]
    limy = [-20,20]
    ax.set_xlim(limx)
    ax.set_ylim(limy)
    ax.set_zlim(-1,2)
    ax.set_xlabel(r"feature 1")
    ax.set_ylabel(r"feature 2")
    
    angles = [(30,30),(25,35),(20,40),(15,45),(10,50),(5,55),(0,63.3)]
    angles2 = [(30,30),(40,30),(50,30),(60,30),(70,30),(80,30),(90,30)]
    angles2.reverse()
    ax.view_init(elev=angles2[0][0],azim=angles2[0][1])
    
    params = [(i*c0/5,i*s/5) for i in range(6)]
    #print(params)
    
    # xline = [-12,8]
    # yline = [-a*(xline[0])/b-c0/b,-a*(xline[1])/b-c0/b]
    # x_line = np.array(xline)
    # y_line = np.array(yline)
    # z_line = np.array([0.5,0.5])
    # ax.plot3D(x_line,y_line,z_line,linewidth=3,color='r')

    xs = np.linspace(-12,8,100)
    zs = np.linspace(-1,2,50)
    X,Z = np.meshgrid(xs,zs)
    Y = -(a/b)*X-(c0/b)
    
    x = np.random.uniform(limx[0],limx[1],200)
    y = np.random.uniform(limy[0],limy[1],200)
    z = np.floor(0.5+func(x,y,a,b,c0,s))
    #print(z)
    
    xvec = np.linspace(limx[0],limx[1],1000)
    yvec = np.linspace(limy[0],limy[1],1000)
    xlist, ylist = np.meshgrid(xvec,yvec)
    
    # c_params = np.linspace(0,3,10)
    # n_params = np.linspace(1.,s,10)
    # params = zip(c_params,n_params)


    # zlog = func(xlist,ylist,a,b,c0,0.01)        
    # ax.plot_surface(xlist,ylist,zlog,cmap=cm.viridis)
    # plt.show()
    
    with writer.saving(fig, "fit2d.mp4", dpi=100):

        # First show the data points
        ax.scatter(x,y,z,color='b',alpha=1)
        for i in range(5):
            writer.grab_frame()
            writer.grab_frame()
        for e,f in angles2:
            ax.view_init(elev=e,azim=f)
            writer.grab_frame()
        for i in range(5):
            writer.grab_frame()
            writer.grab_frame()
        for e,f in angles:
            ax.view_init(elev=e,azim=f)
            writer.grab_frame()
        for i in range(5):
            writer.grab_frame()
            writer.grab_frame()
        
        # Add the surface fit
        for c,n in params:
            zlog = func(xlist,ylist,a,b,c,n)

            ax.set_xlim(limx)
            ax.set_ylim(limy)
            ax.set_zlim(-1,2)
            ax.set_xlabel(r"feature 1")
            ax.set_ylabel(r"feature 2")

            ax.scatter(x,y,z,color='b',alpha=1)
            ax.plot_surface(xlist,ylist,zlog,cmap=cm.viridis)
            
            writer.grab_frame()
            plt.cla()
        
        # Add the plane
        for i in range(5):
            ax.set_xlim(limx)
            ax.set_ylim(limy)
            ax.set_zlim(-1,2)
            ax.set_xlabel(r"feature 1")
            ax.set_ylabel(r"feature 2")

            ax.scatter(x,y,z,color='b',alpha=1)
            ax.plot_surface(xlist,ylist,zlog,cmap=cm.viridis)

            if(i%2 == 0):
                ax.plot_surface(X,Y,Z,color='r',alpha=1)
            else:
                ax.plot_surface(X,Y,Z,color='r',alpha=0)
            
            # Double up the frames to slow things down a bit here.
            writer.grab_frame()
            writer.grab_frame()
            plt.cla()

        ax.set_xlim(limx)
        ax.set_ylim(limy)
        ax.set_zlim(-1,2)
        ax.set_xlabel(r"feature 1")
        ax.set_ylabel(r"feature 2")
        
        ax.scatter(x,y,z,color='b',alpha=1)
        ax.plot_surface(xlist,ylist,zlog,cmap=cm.viridis)
        ax.plot_surface(X,Y,Z,color='r',alpha=1)
        
        angles.reverse()
        angles2.reverse()
        for e,f in angles:
            ax.view_init(elev=e,azim=f)
            writer.grab_frame()
        for i in range(5):
            writer.grab_frame()
            writer.grab_frame()
        for e,f in angles2:
            ax.view_init(elev=e,azim=f)
            writer.grab_frame()
        for i in range(5):
            writer.grab_frame()
            writer.grab_frame()
