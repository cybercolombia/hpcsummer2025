import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns

#At which side of the boundary lies the point.
# >0 means counterclockwise, <0 means clockwise
#Boundary defined as ax+by+c=0
#Point defined as x0,y0
def d_to_boundary(x0,y0,a,b,c):
    #closest point on the line to (x0,y0)
    fact1 = b*x0-a*y0
    fact2 = a**2+b**2
    cp_x = (b*fact1-a*c) / fact2
    cp_y = -(a*fact1+b*c) / fact2
    #director vector of the line (pointing to the right)
    dxr = cp_x - 0 if cp_x > 0 else -cp_x - 0
    dyr = cp_y + c if cp_x > 0 else -cp_y - c
    #separation to closest point
    dx = x0 - cp_x
    dy = y0 - cp_y

    ## print(dxr,dyr)
    ## print(dx,dy)
    #return cross product
    return dxr*dy - dyr*dx


def check_region(p):
    #boundary line 1
    a1 = -1.
    b1 = 1.
    c1 = -0.5
    #boundary line 2
    a2 = 0.5
    b2 = 1.
    c2 = -2.
    #boundary line 3
    a3 = 10.
    b3 = 1.
    c3 = -20

    #position w/respect to boundaries
    d1 = d_to_boundary(p[0],p[1],a1,b1,c1)
    d2 = d_to_boundary(p[0],p[1],a2,b2,c2)
    d3 = d_to_boundary(p[0],p[1],a3,b3,c3)
    #print(d1,d2,d3)
    
    #region 1 
    test1 = d1>0. and d2>0. and d3<0.
    #region 2
    test2 = d1<0. and d2>0. and d3>0.
    #region 3
    test3 = d1<0. and d2<0. and d3<0.
    return test1,test2,test3
    

if __name__ == '__main__':
    Ndata = 5000
    points = 3.5*np.random.rand(Ndata,2)

    y = []
    v_points = []
    for p in points:
        reg1,reg2,reg3 = check_region(p)
        if reg1:
            y.append(0)
            v_points.append(p)
        elif reg2:
            y.append(1)
            v_points.append(p)
        elif reg3:
            y.append(2)
            v_points.append(p)

    y = np.asarray(y).reshape(len(y),1)
    v_points = np.asarray(v_points)

    with open("train.txt", "w") as mydata:
        for i in range(y.shape[0]):
            mydata.write("%.4f,%.4f,%1d\n"%(v_points[i,0],v_points[i,1],y[i,0]))
