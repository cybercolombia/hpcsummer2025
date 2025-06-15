from time import perf_counter
from task1 import task

start_time = perf_counter() #Clock with the highest available resolution

task(0)
task(1)

end_time = perf_counter()

print("Time: {0}".format(end_time-start_time))
