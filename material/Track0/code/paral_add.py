from time import sleep, perf_counter
from threading import Thread

# Delay to inflate the execution time of single tasks
delay = 1.e-5

# Add vector's elements from ini to fin-1
def add(x, y, z, ini, fin):
    for i in range(ini, fin):
        sleep(delay)
        z[i] = x[i] + y[i]

# Parallelized vector addition
def par_vector_sum(v1, v2, n_thr):
    try: 
        if len(v1) != len(v2):
            raise Exception('The two vectors do not have the same number of elements.')
        res = [0 for i in range(len(v1))]
        
        n_thr = min(n_thr, len(v1)) #Number of threds <= length of the vectors
        
        chunk_size = len(v1) // n_thr + 1 #No. of tasks for each thread
        
        #Create the threads
        threads = []
        for i in range(n_thr):
            ini = i*chunk_size
            fin = min(ini + chunk_size, len(v1))
            threads.append(Thread(target=add, args=(v1, v2, res, ini, fin)))
        
        # Start the threads
        for thr in threads:
            thr.start()

        # Wait for the threads to complete
        for thr in threads:
            thr.join()
        return res, n_thr
    
    except Exception as error:
        print(error)
    
# Measure the average execution time of a parallel run
def par_run(v1, v2, Ntrials, n_thr):
    start_time = perf_counter()
    for i in range(Ntrials):
        res, n_thr = par_vector_sum(v1,v2,n_thr)
    end_time = perf_counter()
    
    return res, (end_time-start_time)/Ntrials