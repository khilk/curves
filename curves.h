#pragma once

#include <ostream>

struct Point {
  double x = 0;
  double y = 0;
  double z = 0;
};

std::ostream& operator<<(std::ostream& os, const Point& p);

class Curve {
 public:
  virtual ~Curve() noexcept = default;

  [[nodiscard]] virtual Point get_point(double t) const = 0;
  [[nodiscard]] virtual Point get_derivative(double t) const = 0;
};

class Circle : public Curve {
 public:
  explicit Circle(double radius) noexcept;

  [[nodiscard]] Point get_point(double t) const noexcept final;
  [[nodiscard]] Point get_derivative(double t) const noexcept final;
  [[nodiscard]] double get_radius() const noexcept;

 private:
  double radius_;
};

class Ellipse : public Curve {
 public:
  Ellipse(double radius_x, double radius_y) noexcept;

  [[nodiscard]] Point get_point(double t) const noexcept final;
  [[nodiscard]] Point get_derivative(double t) const noexcept final;

 private:
  double radius_x_;
  double radius_y_;
};

class Helix : public Curve {
 public:
  Helix(double radius, double step) noexcept;

  [[nodiscard]] Point get_point(double t) const noexcept final;
  [[nodiscard]] Point get_derivative(double t) const noexcept final;

 private:
  double radius_;
  double step_;
};
