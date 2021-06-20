#include <stdio.h>
#include <stdlib.h>

typedef int Vectorbasetype;

typedef struct
{
  unsigned int vlength;
  Vectorbasetype *vvalues;
} Vector;

/* Vectorop is the name of the type for functions with
   two Vectorbasetype-arguments and a Vectorbasetype result */
typedef Vectorbasetype (*Vectorop)(Vectorbasetype,Vectorbasetype);

void applyvectorop(Vector *vresult,const Vector *v1,
                   const Vector *v2, Vectorop vop)
{
  unsigned int i;
  if (v1->vlength != v2->vlength)
  {
    fprintf(stderr,"vectors must have equal length\n");
    exit(EXIT_FAILURE);
  }
  vresult->vlength = v1->vlength;
  vresult->vvalues = malloc(vresult->vlength *
                            sizeof *vresult->vvalues);
  if (vresult->vvalues == NULL)
  {
    fprintf(stderr,"Cannot allocate space for result vector\n");
    exit(EXIT_FAILURE);
  }
  for (i = 0; i < vresult->vlength; i++) {
    vresult->vvalues[i] = vop(v1->vvalues[i],v2->vvalues[i]);
  }
}

Vectorbasetype addvalues(Vectorbasetype value1,
                         Vectorbasetype value2) {
  return value1 + value2;
}

Vectorbasetype diffvalues(Vectorbasetype value1,
                          Vectorbasetype value2) {
  return value1 - value2;
}

void vectoradd(Vector *vresult,const Vector *v1,
               const Vector *v2) {
  applyvectorop(vresult,v1,v2,addvalues);
}

void vectordiff(Vector *vresult,const Vector *v1,
                const Vector *v2) {
  applyvectorop(vresult,v1,v2,diffvalues);
}
