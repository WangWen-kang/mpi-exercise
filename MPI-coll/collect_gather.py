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
print myid,'v_lcl',v_lcl

v_glb = np.zeros(nglb,dtype=np.float64)
#         Distribute local vectors v_lcl to their places in v_glb
# Gather local vectors v_lcl to their places in v_glb of process 0 */
# Syntax: comm.Gatherv( sendbuf, recvbuf,root=0)
# sendbuf = v_lcl, recvbuf = [v_glb,counts,dspls[0:nproc],MPI.DOUBLE]
    .....

# Broadcast v_glb from from process 0 to all processes
# Syntax: comm.Bcast(v_glb, root=0)

# Syntax for allgatherv: comm.Allgatherv( sendbuf, recvbuf)

print myid,'v_glb  ',v_glb
