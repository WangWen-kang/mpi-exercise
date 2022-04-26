#!/usr/bin/python
import math 
import time
import numpy as np
from numapp import numapp

piexact = 3.14159265358979323846264338327950288419
n = int(input('number of strips ?   '))
nin = int(input('number of intervals ?   '))
print (" calculation of pi using numerical integration of sqrt(1-x**2)") 
print (" number of strips         : %i" % n) 
print (" number of subintervals   : %i" % nin)

width = 1./nin
pia = 0.
for i in range(nin) :
   lo = width*i
   hi = lo + width 
   res = numapp(lo,hi,n)
   pia = pia + res
print (" pi exact          = %20.15f" % piexact)
print (" pi estimated      = %20.15f \n" % pia)
print (" error             = %20.3e \n" % abs(pia-piexact))
 
