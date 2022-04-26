#errexit.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

def work(iw):
   myid = comm.Get_rank()
   iex = 0
   if iw+myid > 10:
      iex = -1
   return iex

iw = None
if myid == 0:
   print ("iw = ?")
   iw = int(input())
iw = comm.bcast(iw,root=0)
for il in np.arange(iw):
   iex = work(il)
   iexglobal = comm.reduce(iex,root=0,op=MPI.MIN)
   iexglobal = comm.bcast(iexglobal,root = 0)
   if iexglobal < 0:
      print ('error exit on ',myid,' is:', il, iex, iexglobal)
      break
if iexglobal >= 0:
   print ('normal program end ')
