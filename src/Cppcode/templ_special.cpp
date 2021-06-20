#include <iostream>
#include <cctype>

template <class T>
class mycontainer {
  T value;
  public: mycontainer (T arg) { value = arg; }
          T increase () { return ++value; }
};

// class template specialization for T = char
template <>
class mycontainer <char> {
  char value;
  public: mycontainer (char arg) { value = arg; }
          char uppercase () {
            if (value >='a' && value <= 'z')
              return toupper((int) value);
            return value;
          }
};

int main(void) {
  mycontainer<int> myint (7);
  mycontainer<char> mychar ('j');
  std::cout << myint.increase() << std::endl;
  std::cout << mychar.uppercase() << std::endl;
  return EXIT_SUCCESS;
}
