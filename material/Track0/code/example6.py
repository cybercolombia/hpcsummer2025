from seq_add import seq_run

# Sequential test

# Declare and initialize the input
sz = 1000
v1 = [1 for i in range(sz)]
v2 = [2 for i in range(sz)]

Ntrials = 100

res, seq_time = seq_run(v1, v2, Ntrials)

##Check result
assert res == [3 for i in range(sz)]

print("Secuential addition.")
print("Time: {0:.7f}".format(seq_time))