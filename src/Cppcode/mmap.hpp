#ifndef MAPPEDFILE_H
#define MAPPEDFILE_H
#include <cstdint>
class Mappedfile {
  private:
    const char *filename;
    int filedesc;
    uint64_t filesize,
             numberoflines,
             numberofwords;
    bool hasfilevalues;
    void *memorymap;
    void openfile(const char *filename);
    void creatememorymap(void);
    void filevalues_get(void);
  public:
    Mappedfile(const char *filename); /* contructor */
    ~Mappedfile(void);                /* destructor */
    const char *filename_get(void) const;   /* accessor */
    uint64_t filesize_get(void) const; /* accessor */
    uint64_t numberoflines_get(void);  /* accessor */
    uint64_t numberofwords_get(void);  /* accessor */
    const void *memorymap_get(void) const;  /* accessor */
};
#endif
