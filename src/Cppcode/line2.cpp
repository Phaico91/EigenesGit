#include "point.hpp"

class Line 
{
  public: Point start, end;
          double length_get(void) const { 
            return start.distance_to(end); 
          }
};
