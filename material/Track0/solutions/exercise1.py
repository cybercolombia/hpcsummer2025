from time import perf_counter
from threading import Thread
from task1 import task
    
start_time = perf_counter()

# Create 8 threads, each one with a task function
threads = []
for i in range(8):
    threads.append(Thread(target=task, args=(i,)))

# Start the threads
for thr in threads:
    thr.start()

# Wait for the threads to complete
for thr in threads:
    thr.join()

end_time = perf_counter()

print("Time: {0}".format(end_time-start_time))
