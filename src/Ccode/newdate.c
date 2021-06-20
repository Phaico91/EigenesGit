#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>

typedef struct
{
  uint32_t year;
  uint8_t day, month;
} Date;

#ifdef WITHFAILURE
Date *newdatewrong(uint32_t year,uint8_t month,uint8_t day)
{ /* failure as memory of local variables is returned */
  Date date;

  date.day = day;
  date.month = (char) month;
  date.year = (char) year;
  return &date;
}
#endif

Date *newdate(uint32_t year,uint8_t month,uint8_t day) {
  Date *date = malloc(sizeof *date); /* allocate the memory */

  assert(date != NULL);
  date->day = day;
  date->month = month;
  date->year = year;
  return date;
}

static void showdate(const Date *d)
{
  printf("%02d.%02d.%d\n",d->day,d->month,d->year);
}

int main(void)
{
  Date *d = newdate(2020,4,20);
  showdate(d);
  free(d);
}
