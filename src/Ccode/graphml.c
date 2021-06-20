#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <expat.h>

typedef struct
{
  int eventcount;
} Parseinfo;

void  start_hndl (void *userdata,const char *el,const char **attr)
{
  const char **attr_ptr;
  Parseinfo *parseinfo = (Parseinfo *) userdata;

  printf ("\n%4d: Start tag <%s>\n", parseinfo->eventcount++, el);
  for (attr_ptr = attr; *attr_ptr != NULL; attr_ptr+=2)
  {
    printf("\tmap[%s]=%s\n",attr_ptr[0],attr_ptr[1]);
  }
}

void  end_hndl (void *userdata,const char *el)
{
  Parseinfo *parseinfo = (Parseinfo *) userdata;
  printf ("%4d: End tag </%s>\n", parseinfo->eventcount++, el);
}

void  char_hndl (void *userdata,const char *txt, int txtlen)
{
  Parseinfo *parseinfo = (Parseinfo *) userdata;
  printf ("%4d: Text: ",parseinfo->eventcount++);
  fwrite (txt, txtlen, sizeof (char), stdout);
  printf("\n");
}

int  main(int argc,char **argv)
{
  FILE *fpin;
  XML_Parser p;
  Parseinfo parseinfo;
  int done = false;

  if (argc != 2)
  {
    fprintf(stderr,"Usage: %s <graphml-file>\n",argv[0]);
    exit(EXIT_FAILURE);
  }
  fpin = fopen(argv[1],"rb");
  if (fpin == NULL)
  {
    fprintf(stderr,"%s: cannot open file %s\n",argv[0],argv[1]);
    exit(EXIT_FAILURE);
  }
  p = XML_ParserCreate (NULL);
  if (p == NULL)
  {
    fprintf (stderr, "%s: cannot allocate memory for xml-parser\n",argv[0]);
    exit (EXIT_FAILURE);
  }
  XML_SetElementHandler (p, start_hndl, end_hndl);
  XML_SetCharacterDataHandler (p, char_hndl);
  parseinfo.eventcount = 0;
  XML_SetUserData(p, &parseinfo);
  while (!done)
  {
    char buf[1024];
    int len;

    if (fgets(buf, sizeof buf, fpin) == NULL) {
      if (ferror(fpin))
      {
        fprintf (stderr, "Read error\n");
        exit (EXIT_FAILURE);
      }
    }
    len = (int) strlen (buf);
    done = feof(fpin);
    if (!XML_Parse (p, buf, len, done) && !done)
    {
      fprintf (stderr, "Parse error at line %lu:\n%s\n",
                (unsigned long) XML_GetCurrentLineNumber (p),
                XML_ErrorString (XML_GetErrorCode (p)));
      exit (EXIT_FAILURE);
    }
  }
  XML_ParserFree(p);
  fclose(fpin);
  exit (EXIT_SUCCESS);
}
