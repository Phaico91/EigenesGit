#!/usr/bin/env python3

import sys, argparse
#lst{mandelbrotImage}
import numpy as np
from matplotlib import pyplot as plt
plt.switch_backend('agg') # disable X-server

def image_from_file(outputfile,stream,this_cmap):
  matrix = np.loadtxt(stream,dtype=int) # read int-matrix
  fig, ax = plt.subplots()
  ax.matshow(matrix,cmap=this_cmap)
  fig.savefig(outputfile)
#lstend#

def parse_arguments():
  p = argparse.ArgumentParser(description=('generate image from file with '
                                           'matrix of integer values'))
  p.add_argument('-c','--color',action='store_true',default=False,
                  help='generate colored image')
  p.add_argument('inputfile',type=str,
                  help='specify input file, - for stdin')
  p.add_argument('outputfile',type=str,
                  help='specify output file')
  return p.parse_args()

args = parse_arguments()

if args.inputfile == "-":
  stream = sys.stdin
else:
  try:
    stream = open(args.inputfile)
  except IOError as err:
    sys.stderr.write('{}: {}\n'.format(sys.argv[0],err))
    exit(1)

this_cmap = 'jet' if args.color else  'gist_yarg'
image_from_file(args.outputfile,stream,this_cmap)
if args.inputfile != "-":
  stream.close()
