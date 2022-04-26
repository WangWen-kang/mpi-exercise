#synch.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

comm.Barrier()
tm = MPI.Wtime() 
t0 = MPI.Wtime() 
comm.Barrier()
dt = MPI.Wtime() - t0
tdmax=comm.reduce(dt,MPI.MAX)
if myid == 0:
   print ("Barriere fuer %i Prozessoren dauert: %e" %(nproc, tdmax))
   print ("Das sind %e Zyklen" % (tdmax*2400.0e+6)) 
