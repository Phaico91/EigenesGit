#include <stdio.h>
#include <stdlib.h>

  typedef enum {Rain, Snow, Sun} Weather_tag;

int main(int argc, char *argv[])
{
  int input;
  /* check that prog. has one non-neg. int argument <= Sun */
  if (argc != 2 || sscanf(argv[1],"%d",&input) != 1 ||
      input < 0 || input > (int) Sun) {
    fprintf(stderr,"%s <weathernum>\nRain=%d,Snow=%d,Sun=%d\n",
            argv[0],(int) Rain,(int) Snow,(int) Sun);
    return EXIT_FAILURE;
  }
  switch ((Weather_tag) input) {
    case Rain: printf("bring umbrella "); /* no break => include next case */
    case Snow: printf("wear jacket\n"); break;
    case Sun:  printf("wear sunscreen\n"); break;
    default:   printf("strange weather\n");
  }
  return EXIT_SUCCESS;
}
