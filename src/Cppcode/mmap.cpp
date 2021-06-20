#include <unistd.h>     // for fstat()
#include <fcntl.h>      // for open()
#include <sys/mman.h>   // for mmap()
#include <sys/types.h>  // for fstat() and open()
#include <sys/stat.h>   // for fstat() and open()
#include <cstdlib>     // for EXIT_FAILURE, exit()
#include <cctype>      // for iswspace()
#include <iostream>     // for std::cerr
#include <cstdint>      // for uint64_t
#include "mmap.hpp"     // for class Mappedfile declaration

void Mappedfile::openfile(const char *filename)
{
  struct stat buf;

  this->filedesc = open(filename,O_RDONLY);
  if (this->filedesc == -1)       // check for error code
  {
    std::cerr << "openfile failed: " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  if (fstat(this->filedesc,&buf) == -1) // get status of file
  {
    std::cerr << "cannot access status of file "
              << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  this->filesize = buf.st_size;  // store size of file
  this->filename = filename; // store filename
}

void Mappedfile::creatememorymap(void)
{
  this->memorymap = mmap(NULL,
                         (size_t) this->filesize,
                         PROT_READ,
                         MAP_SHARED,
                         this->filedesc,
                         (off_t) 0);
  if (this->memorymap == 
       MAP_FAILED)
  {
    std::cerr << "creation of memorymap("
              << this->filename << ") failed" << std::endl;
    exit(EXIT_FAILURE);
  }
}

void Mappedfile::filevalues_get(void)
{
  const unsigned char *sptr, *send;
  bool lastwasspace = false;
  this->numberoflines = this->numberofwords = 0;
  for (sptr = (const unsigned char *) this->memorymap,
       send = sptr + filesize_get();
       sptr < send; sptr++)
  {
    if (*sptr == '\n')
      this->numberoflines++;
    if (iswspace(*sptr))
    {
      if (!lastwasspace)
      {
        this->numberofwords++;
        lastwasspace = true;
      }
    } else lastwasspace = false;
  }
}

Mappedfile::Mappedfile(const char *filename)
{
  this->hasfilevalues = false;
  openfile(filename);
  creatememorymap();
}

Mappedfile::~Mappedfile(void)
{
  if (munmap(this->memorymap,(size_t) this->filesize) != 0)
  {
    std::cerr << "deletion of memorymap("
              << this->filename << ") failed" << std::endl;
    exit(EXIT_FAILURE);
  }
  close(this->filedesc);
}

const char *Mappedfile::filename_get(void) const
{
  return this->filename;
}

uint64_t Mappedfile::filesize_get(void) const
{
  return this->filesize;
}

const void *Mappedfile::memorymap_get(void) const
{
  return this->memorymap;
}

uint64_t Mappedfile::numberoflines_get(void)
{
  if (!this->hasfilevalues)
  {
    filevalues_get();
    this->hasfilevalues = true;
  }
  return this->numberoflines;
}

uint64_t Mappedfile::numberofwords_get(void)
{
  if (!this->hasfilevalues)
  {
    filevalues_get();
    this->hasfilevalues = true;
  }
  return this->numberofwords;
}
