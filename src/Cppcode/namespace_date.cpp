#include <cstdlib>

namespace Date {
  typedef struct {
    int hour, 
        minute; 
  } Time;
}

int main(void) {
  Date::Time today;
  today.hour = 14;
  today.minute = 15;
  today.minute++;
  return EXIT_SUCCESS;
}
