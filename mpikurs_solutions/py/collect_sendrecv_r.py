#gather_vec.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

# Define the partition vector dspls(0:nproc) for a global vector of size nglb
# and the size vector counts(0:nproc-1) with the sizes of local vectors
dspls = np.zeros(nproc+1,dtype=int)
counts = np.zeros(nproc,dtype=int)
dspls[0] = 0
for ip in np.arange(1,nproc+1):
   dspls[ip] = dspls[ip-1]+ 3+(ip-1)
   counts[ip-1] = dspls[ip] - dspls[ip-1]
nglb = dspls[nproc]

# Initialize the local vectors (such that v_glb(i) = i,i = 0,...,nglb-1):
v_lcl = np.zeros(counts[myid],dtype=np.float64)
for i in np.arange(0,counts[myid]):
   v_lcl[i] = dspls[myid]+i
print (myid,'v_lcl',v_lcl)

# Distribute local vectors v_lcl to their places in v_glb
v_glb = np.zeros(nglb,dtype=np.float64)
buf = np.empty(1000,dtype = np.float64)
MPI.Attach_buffer(buf)
#  Every process sends its local vector to all other processes:
for ip in np.arange(0,nproc):
   comm.Bsend(v_lcl,dest=ip,tag=myid)

#  Every process stores local vectors of  other processes at the
#  appropriate location of the global vector:
for ip in np.arange(0,nproc):
   comm.Recv(v_glb[dspls[ip]:dspls[ip+1]],source=ip,tag=ip)

MPI.Detach_buffer()

print (myid,'v_glb  ',v_glb)
