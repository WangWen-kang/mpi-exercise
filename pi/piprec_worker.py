#!/usr/bin/python
from mpi4py import MPI
import math 
import time
import numpy as np
from numapp import numapp

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
me = comm.Get_rank()

piexact = 3.14159265358979323846264338327950288419
n = 10000000
nin = 50
width = 1./nin
if me == 0:
   print (" calculation of pi using numerical integration of sqrt(1-x**2)")
   print (" number of strips      : %i" % n)
   print (" number of subintervals   : %i" % nin)

intv_l = np.zeros(nin+1,dtype=int)
if me > 0:
# ============  Worker-Teil (me>0) =======================
   res = 0.
   nw = -1
   comm.send(res, dest= 0)
   for i in range( nin+1):
      iw = comm.recv(source = 0)
      if iw > nin-1 :
         break
      hi = 1.0 - iw*width
      lo = hi - width 
      res = numapp(lo, hi, n)
      comm.send(res, dest= 0)
      intv_l[i] = iw
      nw = i
   print (me,' >> ',intv_l[0:nw+1])
if me == 0:
# ============  Farmer-Teil (me=0) =======================
   stat = MPI.Status()
   pia = 0.
   for iw in range( nin+nproc-1):
      res = comm.recv(source=MPI.ANY_SOURCE, status=stat)
      pia = pia + res
      ipw = stat.Get_source()
#           write(6,*) me ,' > ',i, is
      comm.send(iw, dest=ipw)
   print ('Naeherung fuer pi =     %20.10f '% pia)
   print ('error     %20.3e  '% abs(piexact-pia))

