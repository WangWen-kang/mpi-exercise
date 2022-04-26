import numpy as np
from numpy import *
from time import time
from mv import mv

m = 1000  # no. of A's rows
n = 1000  # no. of A's columns and x's rows
A = random.rand(m,n)
x = random.rand(n)
print (' mv.py')
y = zeros(m)
t = time()
y = mv(1,A,x)
ti = time() - t
print (ti, 2*n*m*1.e-6/ti, '{MFlop/s]') 
print ()

print (' numpy matmul')
y = zeros(m)#[:, newaxis]
t = time()
y = np.matmul(A,x)
ti = time() - t
print (ti, 2*n*m*1.e-6/ti, '[MFlop/s]')
print ()

print (' numpy dot: one loop over rows')
y = zeros(m)#[:, newaxis]
t = time()
for i in range(m): 
    y[i] = np.dot(A[i,:], x)
ti = time() - t
print (ti, 2*n*m*1.e-6/ti, '[MFlop/s]')
print ()
