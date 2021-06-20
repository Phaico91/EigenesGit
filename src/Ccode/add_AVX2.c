#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

void add_float_vectors(float *first_array, const float *second_array,
                       size_t len)
{
  for (size_t i = 0; i < len; ++i)
  {
    first_array[i] += second_array[i];
  }
}

void add_double_vectors(double *first_array, const double *second_array,
                        size_t len)
{
  for (size_t i = 0; i < len; ++i)
  {
    first_array[i] += second_array[i];
  }
}

void add_float_vectors_AVX2(float *first_array, const float *second_array,
                            size_t len)
{
  size_t i = 0;
  if (len >= 8)
  {
    for (/*No init*/; i <= len - 8; i += 8)
    {
      __m256 first_values = _mm256_load_ps(&first_array[i]);
      __m256 second_values = _mm256_load_ps(&second_array[i]);
      first_values = _mm256_add_ps(first_values, second_values);
      _mm256_store_ps(&first_array[i], first_values);
    }
  }
  for (; i < len; ++i)
  {
    first_array[i] += second_array[i];
  }
}

float sum_float_vector(const float *array,size_t len)
{
  float this_sum = 0;

  for (size_t i = 0; i < len; ++i)
  {
    this_sum += array[i];
  }
  return this_sum;
}

void add_double_vectors_AVX2(double *first_array, const double *second_array,
                            size_t len)
{
  size_t i = 0;
  if (len >= 4)
  {
    for (/*No init*/; i <= len - 4; i += 4)
    {
      __m256 first_values = _mm256_load_pd(&first_array[i]);
      __m256 second_values = _mm256_load_pd(&second_array[i]);
      first_values = _mm256_add_pd(first_values, second_values);
      _mm256_store_pd(&first_array[i], first_values);
    }
  }
  for (; i < len; ++i)
  {
    first_array[i] += second_array[i];
  }
}

double sum_double_vector(const double *array,size_t len)
{
  double this_sum = 0;

  for (size_t i = 0; i < len; ++i)
  {
    this_sum += array[i];
  }
  return this_sum;
}

int main(int argc,char *argv[])
{
  long i, len;
  double *vec1, *vec2;
  int iter;

  if (argc != 2 || sscanf(argv[1],"%ld",&len) != 1 || len < 1)
  {
    fprintf(stderr,"Usage: %s <len>\n",argv[0]);
    exit(EXIT_SUCCESS);
  }
  srand48(34143347);
  vec1 = calloc(2 * len,sizeof *vec1);
  vec2 = vec1 + len;
  for (i = 0; i < len; i++)
  {
    vec2[i] = drand48();
  }
  for (iter = 0; iter < 10; iter++)
  {
    add_double_vectors_AVX2(vec1,vec2,len);
    //add_double_vectors(vec1,vec2,len);
  }
  printf("sum=%.10e\n",sum_double_vector(vec1,len));
  free(vec1);
  return EXIT_SUCCESS;
}
