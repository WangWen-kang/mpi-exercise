# hello_mpi
from mpi4py import MPI

comm = MPI.COMM_WORLD
size = comm.Get_size()
rank = comm.Get_rank()
name = MPI.Get_processor_name()
print size, rank, name
if rank==0:
   version =  MPI.Get_version()
   print version

