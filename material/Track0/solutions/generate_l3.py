from numpy import random 
from sys import argv

def generate_list(size):
    random.seed(123)
    l = random.randint(0,4,size=size)

    count3 = 0
    for i in l:
        if i == 3: count3 +=1
    return l, count3


if __name__ == '__main__':
    cod, size = argv
    size = int(size)
    
    l, n = generate_list(size)
    print("No. of 3s: ",n)
    with open("solutions/list.dat","w") as outfile:
        for i in l:
            outfile.write("{0} ".format(i))
