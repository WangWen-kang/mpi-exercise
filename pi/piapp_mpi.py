#!/usr/bin/python
from mpi4py import MPI
import math 
import time

# 1) set up mpi environment
comm = MPI.COMM_WORLD
np = comm.Get_size()
me = comm.Get_rank()

# 2) for me=0 : read the value for n from standard input
if me == 0:
   piexact = 3.14159265358979323846264338327950288419
   print ('number of strips ? ')
   n = int(input())
   print (" calculation of pi using numerical integration of sqrt(1-x**2) \n number of strips    :  %i  \n number of processes %i \n" % (n, np))

# 3) distribute n to all processes
if me == 0:
#               np-1 SEND's of n in task 0
else:
#              1 RECV of n in task 1,...,np-1
# 4) starting from strip 1+me sum every np-th strip on task me
delt = 1./n
app = 0.0
# for i in range(...):
   xi = (i+0.5)*delt;
   app = app +  math.sqrt(1.-xi*xi)
pial = 4.*delt*app;
  
# 5) collect all partial sums on task 0
if me>0 :
#             1 SEND of pial in tasks 1,.,np-1
   comm.send(pial,dest=0) 
else :
   pia = pial
#      np-1 RECV's of pial in task 0, adding up pial to pia

# 6) for me=0: write results to standard output
if me == 0 :
   print(" pi exact          = %20.15f" % piexact)
   print(" pi estimated      = %20.15f \n" % pia)
   print(" error             = %20.3e \n" % abs(pia-piexact))
 
