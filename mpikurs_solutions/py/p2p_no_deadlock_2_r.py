#p2p_deadlock.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

for i in np.arange(1,9):
   nl = 10**i
   vec = myid*np.ones(nl,dtype = np.int)
   rvec = np.empty(nl,dtype = np.int)
   print ('size ',nl,'start value ',myid, '>> ',vec[1])
   ipto = (myid +1)%nproc
   ipfr = (myid-1)%nproc
   ntemp = nl + MPI.BSEND_OVERHEAD
   temp = np.empty(ntemp,dtype = np.int)
   MPI.Attach_buffer(temp)
   comm.Bsend(vec,dest=ipto)
   comm.Recv(rvec,source=ipfr)
   MPI.Detach_buffer()
   print ('size ',nl,'value after ring shift',myid, '>> ',rvec[1])


