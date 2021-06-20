unsigned int sumup
        (unsigned short i)
{
  unsigned short j;
  unsigned int sum = 0;

  for (j = 1; j <= i; j++)
  {
    sum += j;
  }
  return sum;
}
