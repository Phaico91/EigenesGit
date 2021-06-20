class Person
{
  ...
  public: void print(void);
          void name_set(char *);
};

class Employee : public Person { ...
    public: void print(void);
            void salary_set(float);
};
