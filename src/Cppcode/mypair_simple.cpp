template <class T>
class Mypair {
    T values[2]; /* array of 2 values of template type T */
  public:
    Mypair (T first, T second) {
      values[0] = first; values[1] = second;
    }
};
