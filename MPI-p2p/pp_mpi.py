#!/usr/bin/python
#p2p_pyobj.py
from mpi4py import MPI
import numpy as np

comm = MPI.COMM_WORLD
nproc = comm.Get_size()
myid = comm.Get_rank()

iw = 20
tlat = 0.000003
speedg = 5.e9
tmin = 0.1
speedmax = np.zeros(nproc)
lat = np.zeros(nproc)
for ip in np.arange(1,nproc):
   if myid == 0:
      print ("    size     reps     ttot        latency       speed")
      print ("  [ Byte]            [secs]        [secs]    [MByte/secs] ")
   for il in np.arange(iw+1):
      len = 2**il
      tg = 2*tlat+16*len/speedg
      lrep = 1+tmin/tg
      snd_vec = np.arange(len,dtype = np.float64)
      rcv_vec = np.zeros(len,dtype = np.float64)
      vec = np.empty(len,dtype = np.float64)
      comm.Barrier()
      
      t0 = MPI.Wtime()
      for irep in np.arange(lrep):
         if myid == 0:
            comm.Send(snd_vec, dest=ip)
            comm.Recv(rcv_vec, source=ip)
         if myid == ip:
            comm.Recv(vec, source=0)
            comm.Send(vec, dest=0)
      dt = MPI.Wtime() - t0
      if myid == 0:
         if dt == 0.0:
            speed = 0.
         else:
            speed = 16.*len*lrep/dt*1.e-6
         if il == 0:
            lat[ip] = dt/(2*lrep)
         if speed >= speedmax[ip]:
            speedmax[ip] = speed
         print ("%8i %8i %12.7f %12.7f %10.3f" % (8*len, lrep, dt, dt/(2*lrep),speed))

comm.Barrier()
name = MPI.Get_processor_name()
if myid == 0:
   print ("\n pingpong summary \n system %s \n number of procs %i" %(name,nproc))
   print (" pingpong between proc 0 and proc ip:")
   print (" ip  latency [s]  bandwidth [GB/secs]")
   for ip in np.arange(1,nproc):
     print (" %3i   %9.2e    %8.2f" % (ip, lat[ip], speedmax[ip]/1000.))


