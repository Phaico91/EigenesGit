#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
  uint32_t year;
  uint8_t day, month;
} Date;

Date *date_tab_parse(unsigned long *numofdates)
{
  int y, m, d; /* dates have format yyyy-mm-dd */
  unsigned long ln, nextfree = 0, allocated = 0;
  Date *date_tab = NULL, *date_ptr;
  /* read from stdin */
  for (ln = 1; scanf("%d-%d-%d\n",&y,&m,&d) == 3; ln++)
  {
    if (y < 0 || m < 1 || m > 12 || d < 1 || d > 31)
    {
      fprintf(stderr,"line %lu: illegal number in date\n",ln);
      exit(EXIT_FAILURE);
    }
    if (nextfree >= allocated)
    {
      allocated += (allocated * 0.2) + 128; /* expon. incr. */
      date_tab = realloc(date_tab,
                         allocated * sizeof *date_tab);
    }
    date_ptr = date_tab + nextfree;
    date_ptr->year = (uint32_t) y;
    date_ptr->month = (uint8_t) m;
    date_ptr->day = (uint8_t) d;
    nextfree++;
  }
  *numofdates = nextfree;
  return date_tab;
}

int date_compare(const void *a,const void *b)
{
  const Date *da = (const Date *) a;
  const Date *db = (const Date *) b;

  if (da->year < db->year)
    return -1;
  if (da->year > db->year)
    return 1;
  if (da->month < db->month)
    return -1;
  if (da->month > db->month)
    return 1;
  if (da->day < db->day)
    return -1;
  if (da->day > db->day)
    return 1;
  return 0;
}

int main(void)
{
  unsigned long numofdates;
  Date *date_tab, *date_ptr;

  date_tab = date_tab_parse(&numofdates); /* from stdin */
  qsort(date_tab,numofdates,sizeof *date_tab,date_compare);
  for (date_ptr = date_tab; date_ptr < date_tab + numofdates;
       date_ptr++)
  {
    printf("%u-%02u-%02u\n",date_ptr->year,
                            date_ptr->month,
                            date_ptr->day);
  }
  free(date_tab);
  return EXIT_SUCCESS;
}
