#include "point.hpp"

class Circle {
 private:
  Point center;
  const double radius;

 public:
  Circle(const Point & centerPoint, double rad);
  void move(double dx, double dy);
  double intersectionArea(const Circle & otherCircle);
};
