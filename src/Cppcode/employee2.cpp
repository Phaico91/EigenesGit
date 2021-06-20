class Employee : public Person { ...
    public: void print();  // from Employee
            void name_set(char *);  // from Person
            void salary_set(float); // from Employee
};
