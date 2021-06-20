#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <inttypes.h>

static char *read_fasta_format(unsigned long *length,FILE *fp)
{
#define MAXLINE 100
  char *sequence = NULL, line[MAXLINE+1];
  unsigned long allocated = 0, nextfree = 0;

  while (fgets(line,MAXLINE,fp) != NULL)
  {
    if (*line != '>')
    {
      char *ptr;
      for (ptr = line; *ptr != '\n'; ptr++)
      {
        if (nextfree >= allocated)
        {
          allocated = allocated * 1.2 + 4;
          sequence = realloc(sequence,allocated * sizeof *sequence);
          assert(sequence != NULL);
        }
        assert(nextfree < allocated);
        sequence[nextfree++] = *ptr;
      }
    }
  }
  *length = nextfree;
  return sequence;
}

static void print_fasta_format(const char *sequence,unsigned long length)
{
  unsigned long idx, lineoffset = 0;

  for (idx = 0; idx < length; idx++)
  {
    if (lineoffset >= 70)
    {
      printf("\n");
      lineoffset = 0;
    }
    putchar(sequence[idx]);
    lineoffset++;
  }
  printf("\n");
}

typedef struct
{
  const char *name;         /* the name of the translation */
  unsigned int identity;    /* the identity number */
  const char *aminos,       /* array of amino acids in order of codons,
                               star stands for stop codon */
             *startcodons;  /* the start codons, represented by Ms in string
                               filled with - */
  uint8_t map[UCHAR_MAX+1]; /* map of bases to numbers in range 0..3,
                               sorted in alphabet order T < C < A < G  */
} TranslationScheme;

static void init_char_map(uint8_t *map)
{
  int idx;

  for (idx = 0; idx <= UINT8_MAX; idx++) {
    map[idx] = UINT8_MAX;
  }
  map['T'] = 0;
  map['t'] = 0;
  map['U'] = 0;
  map['u'] = 0;
  map['C'] = 1;
  map['c'] = 1;
  map['A'] = 2;
  map['a'] = 2;
  map['G'] = 3;
  map['g'] = 3;
}

unsigned int codon2code(const uint8_t *map,const char *codon)
{
  uint8_t cc_0 = map[(int) codon[0]],
          cc_1 = map[(int) codon[1]],
          cc_2 = map[(int) codon[2]];

  assert(cc_0 < 4 && cc_1 < 4 && cc_2 < 4);
  return 16 * cc_0 + 4 * cc_1 + cc_2;
}

/* map T0->A2, C1->G3, A2->T0, G3->C1 */

uint8_t wcc_map(uint8_t cc) { return (cc + 2) % 4; }
unsigned int codon2code_rc(const uint8_t *map,
                           const char *codon) {
  uint8_t cc_0 = map[(int) codon[2]], /* reverse order */
          cc_1 = map[(int) codon[1]],
          cc_2 = map[(int) codon[0]];
  cc_0 = wcc_map(cc_0);
  cc_1 = wcc_map(cc_1);
  cc_2 = wcc_map(cc_2);
  return 16 * cc_0 + 4 * cc_1 + cc_2;
}

unsigned int codon2code_stepwise(const uint8_t *map,bool forward,
                                 const char *codon)
{
  int idx;
  unsigned int code = 0;

  for (idx = 0; idx < 3; idx++)
  {
    uint8_t m_cc, cc = codon[forward ? idx : 2 - idx];
    m_cc = map[(int) cc];
    if (m_cc == (uint8_t) UINT8_MAX)
    {
      fprintf(stderr,"Illegal character at pos %d in codon\n",idx);
      exit(EXIT_FAILURE);
    }
    if (!forward)
    {
      m_cc = wcc_map(m_cc);
    }
    assert(m_cc < 4);
    code = (code * 4) + m_cc;
  }
  if (code >= 64)
  {
    fprintf(stderr,"code=%u,codon=%3.3s\n",code,codon);
    exit(EXIT_FAILURE);
  }
  return code;
}

const TranslationScheme *translationscheme_new(void)
{
  static TranslationScheme schemetable[] = {
    {"Standard",
     (unsigned int) 1,
     "FFLLSSSSYY**CC*WLLLLPPPPHHQQRRRRIIIMTTTTNNKKSSRRVVVVAAAADDEEGGGG",
     "---M---------------M---------------M----------------------------"}
  };
  init_char_map(schemetable[0].map);
  return schemetable;
}

char *dna2peptide(unsigned long *peptide_length,const char *dnaseq,
                  unsigned long len,bool forward)
{
  if (len < 3)
  {
    *peptide_length = 0;
    return NULL;
  } else
  {
    const char *codon;
    const TranslationScheme *scheme = translationscheme_new();
    char *peptide, *peptide_ptr;

    peptide_ptr = peptide = malloc(len/3 * sizeof *peptide);
    assert(peptide != NULL);
    if (forward)
    {
      for (codon = dnaseq; codon < dnaseq + len - 2; codon += 3)
      {
        unsigned int code = codon2code(scheme->map,codon);
        *peptide_ptr++ = scheme->aminos[code];
      }
    } else
    {
      for (codon = dnaseq + len - 3; codon >= dnaseq; codon -= 3)
      {
        unsigned int code = codon2code_rc(scheme->map,codon);
        *peptide_ptr++ = scheme->aminos[code];
      }
    }
    *peptide_length = (unsigned long) (peptide_ptr - peptide);
    return peptide;
  }
}

int main(int argc,char *argv[])
{
  FILE *fp;
  char *dnaseq;
  int idx;
  unsigned long length;

  if (argc != 2)
  {
    fprintf(stderr,"Usage: %s <inputfile>\n",argv[0]);
    return EXIT_FAILURE;
  }
  const char *inputfile = argv[1];
  fp = fopen(inputfile,"rb");
  if (fp == NULL)
  {
    fprintf(stderr,"%s: cannot open file %s\n",argv[0],inputfile);
    return EXIT_FAILURE;
  }
  dnaseq = read_fasta_format(&length,fp);
  fclose(fp);
  fprintf(stderr,"read sequence of length %lu\n",length);
  for (idx = 0; idx < 2; idx++)
  {
    char *peptide;
    unsigned long peptide_length;
    bool forward = (idx == 0) ? true : false;

    peptide = dna2peptide(&peptide_length,dnaseq,length,forward);
    fprintf(stderr,"%s: translate DNA sequence of length %lu from file %s "
                   "to protein sequence of length %lu, forward=%s\n",
            argv[0],
            length,
            inputfile,
            peptide_length,
            forward ? "true" : "false");
    printf(">%stranslation of sequence from %s\n",
           forward ? "" : "reverse complement ",inputfile);
    print_fasta_format(peptide,peptide_length);
    free(peptide);
  }
  free(dnaseq);
  return EXIT_SUCCESS;
}
