#include <cstdbool>
#include "point.hpp"

class Line {
  private: bool changed; /* has point been changed? */
           Point start, end;
           double length; /* update after change */
  public: void start_set(Point p) { start = p;
                                    changed = true;}
    void end_set(Point p) { end = p; changed = true;}
    Point start_get(void) const { return start; }
    Point end_get(void) const { return end; }
    double length_get(void) /* cannot be const */ {
      if (changed) {
        length = start.distance_to(end);
        changed = false;
      }
      return length;
    }
};
