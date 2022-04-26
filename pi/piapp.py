#!/usr/bin/python
import math 
import time

piexact = 3.14159265358979323846264338327950288419
n = int(input('number of strips ?   '))
print (" approximate calculation of pi \n with %i strips " % n)
delt = 1./n
app = 0.0
for i in range(n):
   xi = (i+0.5)*delt;
   app = app + math.sqrt(1.-xi*xi)
pia = 4.*delt*app;

print (" pi exact          = %20.15f" % piexact)
print (" pi estimated      = %20.15f \n" % pia)
print (" error             = %20.3e\n" % abs(pia-piexact))
 
