#!/usr/bin/env python3

import re, sys, subprocess, shlex

splitvalue = 0.51
result = list()
for idx in range(0,49):
  cmd_line = "./golden_search.x -s {}".format(splitvalue)
  cmd_args = shlex.split(cmd_line)
  thispipe = subprocess.Popen(cmd_args,
                              stdout=subprocess.PIPE)
  iterations = None
  out, _ = thispipe.communicate()
  line = str()
  for cc in out.decode():
    if cc == '\n':
      m = re.search(r'^iterations=(\d+)$',line)
      if m:
        iterations = int(m.group(1))
      else:
        m = re.search(r'f\(xmin=([\d\.]*)\)=(-[\d\.]*)',line)
        if not m:
          sys.stderr.write('cannot parse line {}\n'.format(line))
          exit(1)
        fvalue = float(m.group(2))
        result.append((fvalue,splitvalue,iterations))
      line = ""
    else:
      line += cc
  splitvalue += 0.01

for fvalue,splitvalue,iterations in result:
  print("{:.20f}\t{:.2f}\t{}".format(fvalue,splitvalue,iterations))
