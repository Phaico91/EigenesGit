#ifndef PERSON_H
#define PERSON_H

class Person {
  private: 
    char *name;
    int age;

  public: 
    void name_set(char *n) { // inline
      this->name = n; // this refers to the object itself
    }
    void age_set(int);
};
#endif
