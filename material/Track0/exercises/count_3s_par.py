from time import perf_counter, sleep
from threading import Thread, Lock


def read_list():
    l = []
    with open("exercises/list.dat","r") as infile:
        l = list(map(int,infile.read().split()))
    return l

def count3s_thread(l, count, lock, id):
    #Declare a private counter
    
    #Add 3s in the part of the vector assigned to this thread
    
    #Accumulate the total count
    
    
    
def count3s(l, count):
    num_threads = 4
    
    #Define the size of the chunk for each thread
    
    
    #Create the threads, each one taking a chunk of the list
          
    #Start the threads

    #Wait fo the threads to complete

    
#---------------------------------------------
l = read_list() 

count = [0]
t_start = perf_counter()
count3s(l, count)
t_stop = perf_counter()

print("Time: {0} s.".format(t_stop-t_start))
print("Number of 3s counted: {0}".format(count[0]))