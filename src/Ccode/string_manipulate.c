#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc,char *argv[])
{
  if (argc >= 2)
  { 
    char *lastname, *ptr, *firstname = argv[1];
    if ((ptr = strchr(firstname,' ')) == NULL)
      fprintf(stderr,"no space after firstname");
    else {
      *ptr = '\0'; /* replace space by \0 */
      ptr++; /* start scanning character after \0 */
      while (isspace(*ptr)) ptr++; /* skip over spaces */
      lastname = ptr; /* found lastname */
      while (!isspace(*ptr)) ptr++; /* skip over non-spaces */
      *ptr = '\0'; /* end of lastname */
      printf("Your name: \"%s\", \"%s\"\n",lastname,firstname);
    }
  }
  return 0;
  {
    char *lastname, *ptr, *firstname = argv[1];
    if ((ptr = strchr(firstname,' ')) == NULL)
      fprintf(stderr,"no space after firstname");
    else {
      *ptr = '\0'; /* replace space by \0 */
      ptr++; /* start scanning character after \0 */
      while (isspace(*ptr)) ptr++; /* skip over spaces */
      lastname = ptr; /* found lastname */
      while (!isspace(*ptr)) ptr++; /* skip over non-spaces */
      *ptr = '\0'; /* end of lastname */
      printf("Your name: \"%s\", \"%s\"\n",lastname,firstname);
    }
  }
  return 0;
}
