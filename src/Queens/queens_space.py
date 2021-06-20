#!/usr/bin/env python3
import operator as op
from functools import reduce

def binom(n, r):
    r = min(r, n-r)
    numer = reduce(op.mul, range(n, n-r, -1), 1)
    denom = reduce(op.mul, range(1, r+1), 1)
    return numer / denom

valid_combinations = [None,1,0,0,2,10,4,40,92,352,724,2680,14200,73712,365596,2279184]

for n in range(1,15+1):
  print('{}\t{:.2e}\t{:.2e}'.format(n,binom(n*n,n),valid_combinations[n]))
