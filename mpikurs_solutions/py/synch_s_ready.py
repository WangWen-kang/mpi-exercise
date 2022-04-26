#synch_s.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

sbuf = np.ones(1)
rbuf = np.empty(1)
t0 = MPI.Wtime() 
# exchange messages between task 0 and all other tasks
# then send messages from task 0 to all other tasks
#   syntax:
#      comm.Send(sbuf,dest=taskid)
#      comm.Recv(rbuf,source=taskid)
for ip in np.arange(1,nproc):
   if myid == 0:
      comm.Send(sbuf,ip)
   if myid == ip:
      comm.Recv(rbuf,0)
for ip in np.arange(1,nproc):
   if myid == ip:
      comm.Send(sbuf,0)
   if myid == 0:
      comm.Recv(rbuf,ip)

dt = MPI.Wtime() - t0
tdmax=comm.reduce(dt,MPI.MAX)


if myid == 0:
   print ("Barriere fuer %i Prozessoren dauert: %e" %(nproc, tdmax))
   print ("Das sind %e Zyklen" % (tdmax*2400.0e+6))
