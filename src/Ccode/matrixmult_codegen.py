#!/usr/bin/env python3

import sys
import os

#char not supported as no 8bit multiplication in AVX/AVX2
#long not supported as no 64bit multiplication in AVX/AVX2 (posisble to use double?)
#multiplying N-bit values can lead to 2N-bit values -- but these are truncated by AVX2 mullo

sizes = ["256", "512"]
types = ["short", "int", "float", "double"]
counts_256 = {"short":16, "int":8, "float":8, "double":4}

# PARSE ARGS

helpstring = """generate code for vectorized multiplication of matrices
usage: ./{progname} VEC_SIZE VEC_TYPE [optional arguments ...]
\tVEC_SIZE is one of 256, 512
\tVEC_TYPE is one of short, int, float, double
\t[optional] PATH denotes path of output file (default: current dir)""".format(progname=sys.argv[0])

if len(sys.argv) == 1 or sys.argv[1] in ["-h", "--help"]:
    print(helpstring)
    sys.exit()

if len(sys.argv) < 3:
    print(helpstring)
    print("error: VEC_SIZE or VEC_TYPE missing")
    sys.exit(1)

if sys.argv[1] not in sizes:
    print(helpstring)
    print("error: VEC_SIZE does not conform to specification")
    sys.exit(1)


if sys.argv[2] not in types:
    print(helpstring)
    print("error: VEC_TYPE does not conform to specification")
    sys.exit(1)

try:
    path = sys.argv[3]
    if path[-1] != "/":
        path += "/"
except:
    path = os.getcwd() + "/"

if not os.path.isdir(path):
    print(helpstring)
    print("error: PATH does not exist")
    sys.exit(1)

# SET VALS

vec_size = sys.argv[1]
vec_type = sys.argv[2]
vec_len = int(counts_256[vec_type] * (int(vec_size) / 256))

if vec_type == "float":
    dtype = "__m{s}".format(s=vec_size)
    suff = "ps"
    zsuff = suff
    multype = "mul"
elif vec_type == "double":
    dtype = "__m{s}d".format(s=vec_size)
    suff = "pd"
    zsuff = suff
    multype = "mul"
elif vec_type in ["char", "short", "int", "long"]:
    dtype = "__m{s}i".format(s=vec_size)
    suff = "epi{c}".format(c=int(int(vec_size)/vec_len))
    if vec_type == "long" and vec_size == "256":
        suff += "x"
    zsuff = "si256"
    multype = "mullo"

castline = "{type}* {sp}out = ({type}*) &r;".format(sp=" "*4, type=vec_type)

# GET LOAD (VECTOR VALS) AND WRITE (MATRIX VALS) STRINGS

def write_ops(vec_len, jval):
    write_ops = "{spacing}MC(i, {j}) = out[{val}];\n".format(spacing=" "*8,
                                                           val=vec_len - 1,
                                                           j=jval)
    for i in range(vec_len -2, -1, -1):
        write_ops += ("{spacing}MC(i, {j} + {inc}) += out[{val}];\n"
                     .format(spacing=" "*8,inc=vec_len - i - 1,val=i,j=jval))
    return write_ops[:-1]

load_ops = ""
for i in range(1, vec_len - 1):
    load_ops += "\n{spacing}MB(k, j + {val}),".format(spacing=" "*38, val=i)
load_ops += ("\n{spacing}MB(k, j + {val})"
            .format(spacing=" "*38, val=vec_len - 1))

load_ops_trans = ""
for i in range(1, vec_len - 1):
    load_ops_trans += ("\n{spacing}MM(j + {val}, k),"
                      .format(spacing=" "*38, val=i))
load_ops_trans += ("\n{spacing}MM(j + {val}, k)"
                  .format(spacing=" "*38, val=vec_len - 1))


# GET OVERHANG FUNCTION CALLS (IN SWITCH) AND FUNCTIONS

overhang_calls = ""
for i in range(1, vec_len):
    overhang_calls += "{sp1}case {val}:\n" \
                      "{sp2}multiply_{val}overhang(C, A, B, ohang);" \
                      "\n{sp2}break;\n".format(sp1=" "*6, sp2=" "*8, val=i)
overhang_calls = overhang_calls[:-1]

functs = ""
for i in range(1, vec_len):
    overhang_load_ops = "\n"
    for x in range(1, i):
        overhang_load_ops += ("{sp}MB(k, start_j + {val}),\n"
                             .format(sp=" "*32, val=x))
    for y in range(i, vec_len):
        overhang_load_ops += "{sp}0,\n".format(sp=" "*32)
    overhang_load_ops = overhang_load_ops[:-2]

    overhang_write_ops = ("{sp}MC(i, start_j) += out[{val}];\n"
                         .format(sp=" "*4, val=vec_len - 1))
    for z in range(i - 1):
        overhang_write_ops += ("{sp}MC(i, start_j + {inc}) += out[{val}];\n"
                              .format(sp=" "*4, inc=z + 1,val=vec_len - 2 - z))
    overhang_write_ops = overhang_write_ops[:-1]

    functs += """
static void multiply_{val}overhang(Matrix *C, const Matrix *A, const Matrix *B,
                               size_t start_j)
{{
  for (size_t i = 0; i < C->rows; i++)
  {{
    {vectype} r = _mm{vecsize}_setzero_{zerosuff}();
    for (size_t k = 0; k < A->columns; k++)
    {{
      {vectype} a = _mm{vecsize}_set1_{vecsuff}(MA(i, k)),
              b = _mm{vecsize}_set_{vecsuff}(MB(k, start_j),{veccalls}),
              s = _mm{vecsize}_{mul}_{vecsuff}(a, b);
      r = _mm{vecsize}_add_{vecsuff}(r, s);
    }}
{cast}
{vecwrite}
  }}
}}
""".format(val=i, vectype=dtype, vecsize=vec_size, zerosuff=zsuff, mul=multype,
           vecsuff=suff, veccalls=overhang_load_ops,
           vecwrite=overhang_write_ops,
           type=vec_type,
           cast=castline)

# GET OVERHANG SWITCH STATEMENT

overhang = """
  if (C->columns % {veccount} != 0)
  {{
    const size_t ohang = C->columns - (C->columns % {veccount});
    switch (C->columns % {veccount})
    {{
{calls}
      default: assert (false); //something went wrong
    }}
  }}""".format(veccount=vec_len, calls=overhang_calls)

# GET CODE

code = "/* code generated by {} */".format(' '.join(sys.argv))
code += functs
code += """

void matrix_multiplication_vectorized(Matrix *C,const Matrix *A,const Matrix *B)
{{
  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);

  /* square parts of matrix */
  if (C->columns > {coldiff})
  {{
    for (size_t i = 0; i < C->rows; i++)
    {{
      for (size_t j = 0; j < C->columns - {coldiff}; j += {veccount})
      {{
        {vectype} r = _mm{vecsize}_setzero_{zerosuff}();

        for (size_t k = 0; k < A->columns; k++)
        {{
          {vectype} a = _mm{vecsize}_set1_{vecsuff}(MA(i, k)),
                  b = _mm{vecsize}_set_{vecsuff}(MB(k, j),{veccalls}),
                  s = _mm{vecsize}_{mul}_{vecsuff}(a, b);
          r = _mm{vecsize}_add_{vecsuff}(r, s);
        }}
    {cast}
{vecwrite}
      }}
    }}
  }}
{ohang}
}}

void matrix_multiplication_transposed_vectorized(Matrix *C,const Matrix *A,
                                                 const Matrix *B)
{{
  Matrix *M;

  EXPECTED_EQUALITY(A->columns,B->rows);
  EXPECTED_EQUALITY(C->rows,A->rows);
  EXPECTED_EQUALITY(C->columns,B->columns);

  M = matrix_transposition(B);

  /* square parts of matrix */
  if (C->columns > {coldiff})
  {{
    for (size_t i = 0; i < C->rows; i++)
    {{
      for (size_t j = 0; j < C->columns - {coldiff}; j += {veccount})
      {{
        {vectype} r = _mm{vecsize}_setzero_{zerosuff}();

        for (size_t k = 0; k < A->columns; k++)
        {{
          {vectype} a = _mm{vecsize}_set1_{vecsuff}(MA(i, k)),
                  b = _mm{vecsize}_set_{vecsuff}(MM(j, k),{veccalls_trans}),
                  s = _mm{vecsize}_{mul}_{vecsuff}(a, b);
          r = _mm{vecsize}_add_{vecsuff}(r, s);
        }}
    {cast}
{vecwrite}
      }}
    }}
  }}
{ohang}
  matrix_delete(M);
}}
""".format(
coldiff=vec_len - 1,
veccount=vec_len,
vectype=dtype,
vecsize=vec_size,
mul=multype,
type=vec_type,
vecsuff=suff,
zerosuff=zsuff,
veccalls=load_ops,
veccalls_trans=load_ops_trans,
cast=castline,
vecwrite=write_ops(vec_len, "j"),
ohang=overhang)

# WRITE CODE

fname = "matrixmult_vec_{}.c".format(vec_type)
print(path + fname)
with open(path + fname, "w") as f:
    f.write(code)
