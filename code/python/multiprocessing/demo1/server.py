from multiprocessing.managers import BaseManager
import Queue

'''
Taken directly from the examples for multiprocessing. The only purpose for this
file is to serve two queues for clients, of which there are two. 
'''

#Define two queues, one for putting jobs on, one for putting results on.
job_queue = Queue.Queue()
result_queue = Queue.Queue()

#Define a custom class to manage communication
class QueueManager(BaseManager): pass
QueueManager.register('get_job_queue', callable=lambda:job_queue)
QueueManager.register('get_result_queue', callable=lambda:result_queue)

#Start up
m = QueueManager(address=('my.computer.org', 50000), authkey='password')
s = m.get_server()
s.serve_forever()