#include <cstdint>
#include <iostream>
#include <iomanip>

namespace Date {
  struct Time {
    uint16_t hour,
            minute;
  };
}

void time_show(const Date::Time &time)
{ // Time declared in namespace sect.
  std::cout <<
  std::setfill('0') << std::setw(2)
                    << time.hour 
                    << ":" 
                    <<
  std::setfill('0') << std::setw(2)
                    << time.minute 
                    << std::endl;
}
  
void nextminute(Date::Time *time)
{
  time->minute 
    = (time->minute + 1) % 60;
  if (time->minute == 0)
  {
    time->hour 
      = (time->hour + 1) % 24;
  }
}

int main(void)
{
  Date::Time t = {0,0};

  for (int i = 0; i < 24 * 60; i++)
  {
    time_show(t); // call by ref. not visible
    nextminute(&t); // call by ref. visible
  }
}
