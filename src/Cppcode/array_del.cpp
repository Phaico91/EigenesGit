int main(void)
{
  const int entries = 7;
  int *squares = new int[entries];

  for (int i=0; i<entries; i++)
    squares[i] = i * i;
  delete[] squares;
}
