import math
import time
import numpy as np
from zeitschritt import zeitschritt

# problem parameters
nt = int(input())
n = int(input())
r = float(input())
print(nt,n,r)
if r > 0.25:
   print ('Warnung: r =',r,' ! Stabilitaet nur fuer r < 0.25 !') 
eps = 1.e-5

# initialisation
iniv = 1.0
boundaryv = 9.0
a = np.asfortranarray(np.ones((n+2,n+2))*iniv)
u = np.asfortranarray(np.ones((n+2,n+2))*boundaryv)
a[0:1,0:n+2] = boundaryv
a[n+1:n+2,0:n+2] = boundaryv
a[0:n+2,0:1] = boundaryv 
a[0:n+2,n+1:n+2] = boundaryv
# Iteration
t1 = time.time() 
for i in range(nt):
   u = zeitschritt(r,a,u)
   a = zeitschritt(r,u,a)
t2 = time.time() -t1

# Ausgabe
tav = 0.0
for i in range(1,n+1):
   for j in range (1,n+1):
      tav = tav + a[i,j]
skip = max(int((n+1)/20),1)
for i in range(0,n+2,skip):
   for j in range (0,n+2,skip): 
      print ("%2.1f " % (a[i,j]), end ='')
   print ()
nop = 6*n*n
if t2 > 0.0:
   speed = 1.e-6*nop*2*nt/t2
else:
   speed = 0.
print ('grid size: ',n,' x',n)
print ('after ',nt,' timesteps:')
print ('avarage temperatur: %8.5f '% (tav/(n*n)))
print ('run time: %10.4f' % t2)
print ('run speed [Mflop/s] : %5.2f' % speed)


