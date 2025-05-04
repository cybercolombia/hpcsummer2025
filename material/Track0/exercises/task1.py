from time import sleep

def task(n):
    print('Starting task {0}...'.format(n))
    sleep(1)
    print('Task {0}: done'.format(n))
