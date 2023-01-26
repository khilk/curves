#include "curves.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_reduce.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

namespace {

std::vector<std::shared_ptr<Curve>> create_vector_of_curves(std::size_t n) {
  std::vector<std::shared_ptr<Curve>> res;
  res.reserve(n);

  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<int> uni_type(0, 2);
  std::uniform_real_distribution<> uni_radius(0.1, 100);
  std::uniform_real_distribution<> uni_step(0.1, 20);

  for (std::size_t i = 0; i < n; ++i) {
    int type = uni_type(rng);
    switch (type) {
      case 0: {
        res.emplace_back(std::make_shared<Circle>(uni_radius(rng)));
        break;
      }
      case 1: {
        res.emplace_back(std::make_shared<Ellipse>(uni_radius(rng), uni_radius(rng)));
        break;
      }
      case 2: {
        res.emplace_back(std::make_shared<Helix>(uni_radius(rng), uni_step(rng)));
        break;
      }
      default:
        assert(false);
    }
  }
  return res;
}

void print_vector_of_curves(const std::vector<std::shared_ptr<Curve>>& vec, double t = M_PI / 4) {
  for (const auto& item : vec) {
    std::cout << "point: " << item->get_point(t) << ", derivative: " << item->get_derivative(t) << std::endl;
  }
}

std::vector<std::shared_ptr<Circle>> get_circles(const std::vector<std::shared_ptr<Curve>>& vec) {
  std::vector<std::shared_ptr<Circle>> res;
  for (const auto& item : vec) {
    if (auto circle_ptr = std::dynamic_pointer_cast<Circle>(item); circle_ptr) {
      res.push_back(circle_ptr);
    }
  }
  return res;
}

void sort_by_radius(std::vector<std::shared_ptr<Circle>>& vec) {
  std::sort(vec.begin(), vec.end(), [](const std::shared_ptr<Circle>& lhs, const std::shared_ptr<Circle>& rhs) {
    return lhs->get_radius() < rhs->get_radius();
  });
}

enum ExecutionPolicy {
  Sequenced,
  Parallel,
};

template <ExecutionPolicy policy = ExecutionPolicy::Sequenced>
double count_sum_of_radii(const std::vector<std::shared_ptr<Circle>>& vec) {
  static_assert(policy == ExecutionPolicy::Sequenced || policy == ExecutionPolicy::Parallel);

  if constexpr (policy == ExecutionPolicy::Sequenced) {
    return std::accumulate(vec.begin(), vec.end(), 0.,
                           [](double value, const auto& rhs) { return value + rhs->get_radius(); });
  } else if constexpr (policy == ExecutionPolicy::Parallel) {
    return tbb::parallel_reduce(
        tbb::blocked_range<size_t>(0, vec.size()), 0.0,
        [&](tbb::blocked_range<size_t> r, double running_total) {
          for (auto i = r.begin(); i < r.end(); ++i) {
            running_total += vec[i]->get_radius();
          }
          return running_total;
        },
        std::plus<>());
  }
  assert(false);
  return 0;  // just for avoiding static analyzer's warnings
}

}  // namespace

int main() {
  auto curves = create_vector_of_curves(1000000);
  print_vector_of_curves(curves);

  auto circles = get_circles(curves);
  sort_by_radius(circles);

  using namespace std::chrono;
  // count sum of radii without multithreading
  {
    auto start = high_resolution_clock::now();
    std::cout << "sum = " << count_sum_of_radii<ExecutionPolicy::Sequenced>(circles)
              << ", sequenced solution time (microseconds): "
              << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << std::endl;
  }
  // count sum of radii in parallel
  {
    auto start = high_resolution_clock::now();
    std::cout << "sum = " << count_sum_of_radii<ExecutionPolicy::Parallel>(circles)
              << ", parallel solution time (microseconds): "
              << duration_cast<microseconds>(high_resolution_clock::now() - start).count() << std::endl;
  }
  return 0;
}
