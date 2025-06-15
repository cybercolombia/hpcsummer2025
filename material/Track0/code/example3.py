from time import sleep, perf_counter

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

for i in range(8):
    increment_variable(i)

end_time = perf_counter()

print("\nFinal x={0}".format(x))
print("Time: {0}".format(end_time-start_time))