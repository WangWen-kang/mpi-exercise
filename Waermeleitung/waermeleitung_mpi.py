from mpi4py import MPI
import math
import time
import numpy as np
from zeitschritt import zeitschritt
from dist_index import dist_index
from randaustausch import randaustausch

comm = MPI.COMM_WORLD
nproc= comm.Get_size()
myid = comm.Get_rank()

# problem parameters
(nt,n,r) = (0,0,0.0)
if myid==0:
   nt = int(input())
   n = int(input())
   r = float(input())
(nt,n,r) = comm.bcast((nt,n,r),root=0)
if r > 0.25:
   print ('Warnung: r =',r,' ! Stabilitaet nur fuer r < 0.25 !') 

firstind = dist_index(n)
n2l = firstind[myid+1] - firstind[myid]

# initialisation
iniv = 1.0
boundaryv = 9.0
#a = np.ones((n1+2,n2l+2))*iniv
a = np.asfortranarray(np.ones((n+2,n2l+2))*iniv)
u = np.asfortranarray(np.ones((n+2,n2l+2))*boundaryv)
a[0:1,0:n2l+2] = boundaryv
a[n+1:n+2,0:n2l+2] = boundaryv
if myid == 0:
   a[0:n+2,0:1] = boundaryv
if myid == nproc -1: 
   a[0:n+2,n2l+1:n2l+2] = boundaryv
#print myid, a
# Iteration
trand = 0.0
tupd = 0.0
t1 = time.time() 
for i in range(nt):
   c1 = time.time()
   a = randaustausch(n,n2l,a)
   c2 = time.time()
   u = zeitschritt(r,a,u)
   c3 = time.time()
   u = randaustausch(n,n2l,u)
   c4 = time.time()
   a = zeitschritt(r,u,a)
   c5 = time.time()
   trand = trand + c2-c1 + c4-c3
   tupd = tupd + c3-c2 + c5-c4
t2 =  time.time() - t1
# Ausgabe
comm.Barrier()
tavl = 0.0
tav = 0.0
for i in range(1,n+1):
   for j in range (1,n2l+1):
      tavl = tavl + a[i,j]
tav = comm.reduce(tavl, op=MPI.SUM, root=0)
if nproc == 1:
   skip = max(int((n+1)/20),1)
   for i in range(0,n+2,skip):
      for j in range (0,n+2,skip):
         print ("%2.1f " % (a[i,j]), end ='')
      print ()
if myid == 0:
   print ('grid size: ',n,' x',n)
   print ('after ',nt,' timesteps:')
   print ('avarage temperatur: %8.5f '% (tav/(n*n)))
   print ('run time: %10.4f' % t2)
   nop = 6*n*n
   speed = 1.e-6*nop*2*nt/t2
   print ('run speed [Mflop/s] : %5.2f' % speed)
   print ('trand, tupd : %10.4f %10.4f' % (trand,tupd))

