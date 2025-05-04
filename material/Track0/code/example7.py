from sys import argv
from paral_add import par_run

# Parallel test

code, seq_time = argv
seq_time = float(seq_time)

# Declare and initialize the input
sz = 1000
v1 = [1 for i in range(sz)]
v2 = [2 for i in range(sz)]

Ntrials = 100

#Number of threads
nt = 4
res, par_time = par_run(v1, v2, Ntrials, nt)

##Check result
assert res == [3 for i in range(sz)]

print("No. of threds: {0}".format(nt))
print("Time: {0:.7f}".format(par_time))
print("Speedup: {0:.3f}".format(seq_time/par_time))