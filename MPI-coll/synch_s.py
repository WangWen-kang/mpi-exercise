#synch_s.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

sbuf = np.ones(1)
rbuf = np.empty(1)
t0 = MPI.Wtime() 
for i in np.arange(nsynch):
# Example implementation of a barrier (sequential access):
#   all tasks except task 0 send a message ( msg ) to task 0
#   task 0 sends a message to all other tasks
#   syntax:
#      comm.Send(sbuf,dest=taskid)
#      comm.Recv(rbuf,source=taskid)

dt = MPI.Wtime() - t0
tdmax=comm.reduce(dt,MPI.MAX)


if myid == 0:
   print ("Barriere fuer %i Prozessoren dauert: %e" %(nproc, tdmax))
   print ("Das sind %e Zyklen" % (tdmax*2400.0e+6))
