#!/usr/bin/python
import math 
import numpy as np

def numapp(lo,hi,n):
      flo = math.sqrt(1.-lo*lo)
      fhi = math.sqrt(1.-hi*hi)
      nl = int(n * (flo - fhi))
      delt = (hi-lo)/nl
      app = 0.0

      for i in range(0,nl):
         xi = lo + (i+0.5)*delt;
         app = app +  math.sqrt(1.-xi*xi)
      res = 4.*delt*app;
      return (res)
