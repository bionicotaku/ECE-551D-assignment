#include "circle.hpp"

#include <cmath>
// Constructor implementation
Circle::Circle(const Point & centerPoint, double rad) : center(centerPoint), radius(rad) {
}

// move method implementation
void Circle::move(double dx, double dy) {
  center.move(dx, dy);
}

// intersectionArea method implementation
double Circle::intersectionArea(const Circle & otherCircle) {
  double d = center.distanceFrom(otherCircle.center);
  // If one circle is entirely within the other
  if (d + otherCircle.radius <= radius)
    return 3.14159265358979323846 * std::pow(otherCircle.radius, 2);
  if (d + radius <= otherCircle.radius)
    return 3.14159265358979323846 * std::pow(radius, 2);

  // If the circles do not intersect
  if (d >= radius + otherCircle.radius)
    return 0;

  // Otherwise, use the formula to find the intersection area
  double angle1 =
      2 *
      std::acos((std::pow(d, 2) + std::pow(radius, 2) - std::pow(otherCircle.radius, 2)) /
                (2 * d * radius));
  double angle2 =
      2 *
      std::acos((std::pow(d, 2) + std::pow(otherCircle.radius, 2) - std::pow(radius, 2)) /
                (2 * d * otherCircle.radius));

  double area1 = 0.5 * std::pow(radius, 2) * (angle1 - std::sin(angle1));
  double area2 = 0.5 * std::pow(otherCircle.radius, 2) * (angle2 - std::sin(angle2));

  return area1 + area2;
}
