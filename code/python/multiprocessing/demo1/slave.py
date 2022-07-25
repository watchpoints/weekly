from multiprocessing.managers import BaseManager


# Define manager
class QueueManager(BaseManager): pass


QueueManager.register('get_job_queue')
QueueManager.register('get_result_queue')

# Connect to server
m = QueueManager(address=('my.computer.org', 50000), authkey='password')
m.connect()

# Set up queus
job_queue = m.get_job_queue()
result_queue = m.get_result_queue()

while (True):
    job = job_queue.get()

    print(job)

    # Job is always an indexable object
    # It can either be a string identifying the operation
    # Or it can be a real function object!
    # With the caveat that the function must be pickleable

    op = job[0]

    if op == 'add':
        res = job[1] + job[2]
    elif op == 'sub':
        res = job[1] - job[2]
    else:
        res = op(job[1], job[2])

    print("Sending result: " + str(res))
    result_queue.put(res)