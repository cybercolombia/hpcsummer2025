from time import sleep, perf_counter

#Read the list of numbers from a file
def read_list():
    l = []
    with open("solutions/list.dat","r") as infile:
        l = list(map(int,infile.read().split()))
    return l

#Count the number of 3s in list l and store the result on count
def count3s(l, count):
    for i in range(len(l)):
        sleep(1.e-3)
        if l[i] == 3:
            count[0] += 1  
            
#------------------------------------------------------
l = read_list()
    
count = [0]
t_start = perf_counter()
count3s(l, count)
t_stop = perf_counter()

print("Time: {0} s.".format(t_stop-t_start))
print("Number of 3s counted: {0}".format(count[0]))