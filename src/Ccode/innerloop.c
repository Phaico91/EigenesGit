for (j = 0; /* j-loop */
     j < C->columns;
     j++)
{
  Matrixbasetype p, r = 0;

  for (k = 0;
       k < A->columns; /* = n */
       k++) {
/* abbrev: MA,MB,MC by A,B,C */
    p = A(i,k) * B(k,j);
    r = r + p;
  }
  C(i,j) = r;
}
for (j = 0; j < C->columns - 3;
     j += 4) /* step size 4 */
{
  Matrixbasetype p[4], r[4] = {0};

  for (k = 0; k < A->columns;
       k++)
  {
    p[0] = A(i,k) * B(k,j);
    p[1] = A(i,k) * B(k,j+1);
    p[2] = A(i,k) * B(k,j+2);
    p[3] = A(i,k) * B(k,j+3);
    r[0] = r[0] + p[0];
    r[1] = r[1] + p[1];
    r[2] = r[2] + p[2];
    r[3] = r[3] + p[3];
  }
  C(i,j)   = r[0];
  C(i,j+1) = r[1];
  C(i,j+2) = r[2];
  C(i,j+3) = r[3];
}

for (j = 0; j < C->columns - 3; 
     j += 4)
{
  __m256d a, b, p, 
          r = _mm256_setzero_pd(); 
  double *rd = (double*) &r;

  for (k = 0; k < A->columns; k++)
  {
    a = _mm256_set1_pd(A(i, k)),
    b = _mm256_set_pd(B(k,j),
                      B(k,j+1),
                      B(k,j+2),
                      B(k,j+3)),
    p = _mm256_mul_pd(a, b);
    r = _mm256_add_pd(r, p);
  } /* reversed order in rd: */
  C(i, j)     = rd[3]; 
  C(i, j + 1) = rd[2];
  C(i, j + 2) = rd[1];
  C(i, j + 3) = rd[0];
}
