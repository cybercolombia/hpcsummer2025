from seq_add import seq_run
from paral_add import par_run


# Declare and initialize the input
sz = 1000
v1 = [1 for i in range(sz)]
v2 = [2 for i in range(sz)]
Ntrials = 100

#Perform the computations for  different number of cores
res, seq_time = seq_run(v1, v2, Ntrials)

speedups = []
procs = [i for i in range(1,7)]
for P in procs:
    res, par_time = par_run(v1, v2, Ntrials, P)
    speedups.append(seq_time/par_time)
    
#Print results to file
with open("code/spdup_data.dat", "w") as ofile:
    for i in range(len(procs)):
        ofile.write("{0} {1}\n".format(procs[i],speedups[i]))
