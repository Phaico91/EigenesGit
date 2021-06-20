Person::Person(const Person &p)
{
  this->name = p.name;
  this->age = p.age + 1;
}
