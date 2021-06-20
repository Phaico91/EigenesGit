#ifndef POINT_HPP
#define POINT_HPP
#include <cmath>
class Point {
  private: double xcoord, ycoord;
  public: Point(double x,double y) {
            xcoord = x;
            ycoord = y;
          }
          double distance_to(const Point &p) const {
            double xdist = xcoord - p.xcoord;
            double ydist = ycoord - p.ycoord;
            return sqrt(xdist * xdist + ydist * ydist);
          }
};
#endif
