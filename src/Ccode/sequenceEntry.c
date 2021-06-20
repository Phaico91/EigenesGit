#include <stdio.h>
#include <string.h>
#include <stdint.h>
typedef struct
{
  char accession[8+1], /* 1 extra byte for \0 */
       content[500+1]; /* string terminator */
  unsigned long length;
} SequenceEntry;

SequenceEntry seq_entry;

typedef struct {
  uint32_t year; uint8_t month, day;
} Date;

typedef struct {
  char accession[8+1],
       content[500+1];
  unsigned long length;
  Date created;
} EnhancedSequenceEntry;

int main(void)
{
  SequenceEntry seqEntry;
  strcpy(seqEntry.content,"acgatacg");
  printf("content=%s\n",seqEntry.content);
EnhancedSequenceEntry seqdb[50], *last;
seqdb[42].created.year = 2002;
seqdb[42].created.year++;
last = seqdb + sizeof seqdb/sizeof seqdb[0] - 1;
strcpy(last->accession,"P53"); /* usual -> notation */
printf("acc=%s\n",(*last).accession); /* unusual (*last). */
  return 0;
}
