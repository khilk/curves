#include "curves.h"

#include <cmath>

Circle::Circle(double radius) noexcept : radius_(radius) {
}

[[nodiscard]] Point Circle::get_point(double t) const noexcept {
  return {radius_ * cos(t), radius_ * sin(t), 0};
}

[[nodiscard]] Point Circle::get_derivative(double t) const noexcept {
  return {radius_ * (-sin(t)), radius_ * cos(t), 0};
}

Ellipse::Ellipse(double radius_x, double radius_y) noexcept : radius_x_(radius_x), radius_y_(radius_y) {
}

[[nodiscard]] Point Ellipse::get_point(double t) const noexcept {
  return {radius_x_ * cos(t), radius_y_ * sin(t), 0};
}

[[nodiscard]] Point Ellipse::get_derivative(double t) const noexcept {
  return {radius_x_ * (-sin(t)), radius_y_ * cos(t), 0};
}

Helix::Helix(double radius, double step) noexcept : radius_(radius), step_(step) {
}

[[nodiscard]] Point Helix::get_point(double t) const noexcept {
  return {radius_ * cos(t), radius_ * sin(t), t * step_ / (2 * M_PI)};
}

[[nodiscard]] Point Helix::get_derivative(double t) const noexcept {
  return {radius_ * (-sin(t)), radius_ * cos(t), step_ / (2 * M_PI)};
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
  return os << "{" << p.x << ", " << p.y << ", " << p.z << "}";
}

[[nodiscard]] double Circle::get_radius() const noexcept {
  return radius_;
}
