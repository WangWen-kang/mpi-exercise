#bcast.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
me = comm.Get_rank()

# for me=0 : read the input values from standard input
niter = 0
val = 0.
if me == 0 :
   piexact = 3.14159265358979323846264338327950288419
   print ('number of iterations ? ')
   niter = int(input())
   print ('start value ? ')
   val = float(input())

   print (" input values: %i , %f \n" % (niter, val))

# distribute input values to all processes
if me == 0 :
   for ip in np.arange(1,nproc):
      comm.send(niter,dest=ip,tag=0)
      comm.send(val,dest=ip,tag=1)
else :
   niter = comm.recv(source=0,tag=0)
   val = comm.recv(source=0,tag=1)
#  Use MPI_Bcast for distributing input values
#  Syntax:
#  robj = comm.bcast(sobj, root= 0) 

print (" on proc. %i :input values: %i , %f \n" % (me,niter, val))

