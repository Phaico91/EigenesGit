#include <stdlib.h>
#include <stdio.h>

int evaluate_prediction(int a,int b)
{
  if (a >= 130)
  {
    if (b >= 50)
      return 1;
  } else
  {
    if (b >= 70)
      return 1;
  }
  return 0;
}

int main(int argc,char *argv[])
{
  FILE *in_fp;
  size_t tn = 0, fn = 0, fp = 0, tp = 0;
  int a, b, label;

  if (argc != 2)
  {
    fprintf(stderr,"Usage: %s <inputfile>\n",argv[0]);
    return EXIT_FAILURE;
  }
  in_fp = fopen(argv[1],"r");
  if (in_fp == NULL)
  {
    fprintf(stderr,"%s: cannot open file %s\n",argv[0],
                                               argv[1]);
    return EXIT_FAILURE;
  }

  while (fscanf(in_fp,"%d %d %d\n",&a,&b,&label) == 3)
  {
    const int prediction = evaluate_prediction(a,b);
    tn += (prediction == 0 && label == 0) ? 1 : 0;
    fn += (prediction == 0 && label == 1) ? 1 : 0;
    fp += (prediction == 1 && label == 0) ? 1 : 0;
    tp += (prediction == 1 && label == 1) ? 1 : 0;
  }
  printf("tn=%lu,fn=%lu,fp=%lu,tp=%lu,",tn,fn,fp,tp);
  printf("recall=%.2f%%,",100.0 * (double) tp/(tp + fn));
  printf("precision=%.2f%%\n",100.0 * (double) tp/(tp + fp));
  fclose(in_fp);
  return EXIT_SUCCESS;
}
