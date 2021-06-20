#!/usr/bin/env python3

import sys

#lst{EvaluatePredictionFunc}
def evaluate_prediction(a,b):
  if a >= 130:
    if b >= 50:
      return 1
  else:
    if b >= 70:
      return 1
  return 0
#lstend#

#lst{InputFeatures}
if len(sys.argv) != 2:
  sys.stderr.write('Usage: {} <inputfile>\n'
                    .format(sys.argv[0]))
  exit(1)
try:
  in_fp = open(sys.argv[1])
except IOError as err:
  sys.stderr.write('{}: {}\n'.format(sys.argv[0],err))
  exit(1)
#lstend#

#lst{EvaluatePredictionValue}
tn, fn, fp, tp = 0, 0, 0, 0
for line in in_fp:
  values = line.rstrip().split('\t')
  if len(values) < 3:
    break
  a, b, label = map(int,values)
  prediction = evaluate_prediction(a,b)
  tn += (1 if (prediction == 0 and label == 0) else 0)
  fn += (1 if (prediction == 0 and label == 1) else 0)
  fp += (1 if (prediction == 1 and label == 0) else 0)
  tp += (1 if (prediction == 1 and label == 1) else 0)
in_fp.close()
print('tn={},fn={},fp={},tp={},'.format(tn,fn,fp,tp),end='')
print('recall={:.2f}%,'.format(100.0 * tp/(tp + fn)),end='')
print('precision={:.2f}%'.format(100.0 * tp/(tp + fp)))
#lstend#
