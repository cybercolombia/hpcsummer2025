from time import perf_counter
from threading import Thread
from task1 import task
    
start_time = perf_counter()

# Create 8 threads, each one with a task function

# Start the threads

# Wait for the threads to complete

end_time = perf_counter()

print("Time: {0}".format(end_time-start_time))
