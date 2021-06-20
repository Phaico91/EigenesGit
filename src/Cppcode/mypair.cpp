// class templates
#include <cstdint>
#include <iostream>

template <class T>
class Mypair { 
  T values[2];
  public: Mypair (T first, T second) { // inline constructor
            values[0] = first; 
            values[1] = second;
          }
          T getmax ();                // forward declaration
};
template <class T>
T Mypair<T>::getmax () { // member function of template class
  return values[0] > values[1] ? values[0] : values[1];
}

int main(void) {
  Mypair<int> myobject (100, 75); // implicit call of constr.
  std::cout << myobject.getmax() << std::endl;
  return EXIT_SUCCESS;
}
