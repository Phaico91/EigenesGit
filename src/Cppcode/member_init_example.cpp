class Person {
  public: Person(std::string &);
  ...
};

class Workplace {
  Person worker;
  ...
};

Workplace::Workplace(std::string &name) :
  worker(name)
  { ... }
