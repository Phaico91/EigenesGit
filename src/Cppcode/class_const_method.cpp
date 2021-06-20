class Person 
{ 
  private: 
    char *name; 
    int age;

  public: 
    int age_get(void) const 
    {
      return this->age;
    }
};
