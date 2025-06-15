from time import sleep, perf_counter
from threading import Thread

x = 0

def increment_variable(n):
    global x
        
    sleep(1)
    local_x = x
    print("Task {0}: Adding 1 to x={1}".format(n, x))
    local_x += 1
    x = local_x
    print("Task {0}: Finished. x={1}".format(n,x))

#------------------------------------------------------------------
print("Initial x={0}\n".format(x))

start_time = perf_counter()

# Create 8 threads, each one with a task function
threads = []
for i in range(8):
    threads.append(Thread(target=increment_variable, args=(i,)))

# Start the threads
for thr in threads:
    thr.start()

# Wait for the threads to complete
for thr in threads:
    thr.join()

end_time = perf_counter()

print("\nFinal x={0}".format(x))
print("Time: {0}".format(end_time-start_time))
