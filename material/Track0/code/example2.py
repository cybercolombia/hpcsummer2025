from time import perf_counter
from threading import Thread
from task1 import task

start_time = perf_counter()

# Create two threads, each one with a task function
t0 = Thread(target=task, args=(0,))
t1 = Thread(target=task, args=(1,))

# Start the threads
t0.start()
t1.start()

# Wait for the threads to complete
t0.join()
t1.join()

end_time = perf_counter()

print("Time: {0}".format(end_time-start_time))
