#p2p_pyobj.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

if myid == 0:
   osend = [10 + myid,'sent from 0']
   comm.send(osend,dest=1)
   print ('on',myid, 'osend is', osend)
if myid == 1:
   orecv = comm.recv(source=0)
   print ('on',myid, 'orecv is', orecv)


