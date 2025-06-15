from time import perf_counter, sleep
from threading import Thread, Lock


def read_list():
    l = []
    with open("solutions/list.dat","r") as infile:
        l = list(map(int,infile.read().split()))
    return l

def count3s_thread(l, count, lock, id):
    priv_count = 0
    #Add 3s in the part of the vector assigned to this thread
    for i in l:
        if i == 3:
            sleep(1.e-3)
            priv_count += 1
    #Accumulate the total count
    lock.acquire()
    count[0] += priv_count
    lock.release()
    print("Thread {0} has last count at {1}.".format(id,count[0]))
    
def count3s(l, count):
    num_threads = 4
    #Size of the chunk for each thread
    chunk = (len(l)+1) // num_threads

    #Declare a lock object
    lock = Lock()
    
    #Create the threads, each one taking a chunk of the list
    threads = []
    for thr in range(num_threads):
        ini = thr*chunk
        fin = ini+chunk if thr < num_threads-1 else len(l)
        threads.append(Thread(target=count3s_thread, args=(l[ini:fin],count,lock,thr)))

    #Start the threads
    for thr in threads:
        thr.start()

    #Wait fo the threads to complete
    for thr in threads:
        thr.join()
        

#---------------------------------------------
l = read_list() 

count = [0]
t_start = perf_counter()
count3s(l, count)
t_stop = perf_counter()

print("Time: {0} s.".format(t_stop-t_start))
print("Number of 3s counted: {0}".format(count[0]))
    
