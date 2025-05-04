from time import sleep, perf_counter

# Delay to inflate the execution time of single tasks
delay = 1.e-5

# Sequential vector addition
def vector_sum(v1, v2):
    try: 
        if len(v1) != len(v2):
            raise Exception('The two vectors do not have the same number of elements.')
        res = [0 for i in range(len(v1))]
        
        for i in range(len(v1)):
            sleep(delay)
            res[i] = v1[i] + v2[i]
            
        return res
    
    except Exception as error:
        print(error)
        
# Measure the average execution time of a sequential run
def seq_run(v1, v2, Ntrials):
    start_time = perf_counter()
    for i in range(Ntrials):
        res = vector_sum(v1,v2)
    end_time = perf_counter()
        
    return res, (end_time-start_time)/Ntrials




