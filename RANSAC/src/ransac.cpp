#include "RANSAC/ransac.hpp"

auto createDummySet() -> std::vector<Point>
{
  // std::random_device rd;
  std::mt19937 mt(40);
  std::uniform_real_distribution<double> x_dist(0, 100);
  std::uniform_real_distribution<double> noise(-0.05, 0.05);
  std::uniform_real_distribution<double> outliers(-100, 200);
  std::vector<Point> sample_space;
  double slope = 5;
  double intercept = 2;
  for (size_t i = 0; i < 40; ++i)
  {
    auto x = x_dist(mt);
    auto y = (slope * x) + intercept + noise(mt);
    sample_space.emplace_back(x, y);
  }
  for (size_t i = 0; i < 60; ++i)
  {
    auto x = outliers(mt);
    auto y = outliers(mt);
    sample_space.emplace_back(x, y);
  }
  return sample_space;
}
double pointToLineDistance(const Point& p, const Point& a, const Point& b)
{
  double x0 = p.first, y0 = p.second;
  double x1 = a.first, y1 = a.second;
  double x2 = b.first, y2 = b.second;

  double numerator = std::abs((y2 - y1) * x0 - (x2 - x1) * y0 + x2 * y1 - y2 * x1);
  double denominator = std::sqrt((y2 - y1) * (y2 - y1) + (x2 - x1) * (x2 - x1));

  return numerator / denominator;
}
int main()
{
  using vecP = std::vector<Point>;
  // Randomly generate a distribution
  auto sample_space = createDummySet();
  auto model = algorithms::RANSAC<Point, double>(sample_space, 0.05, 0.6, 0.95);

  // because its a line fitting problem the number of samples is 2
  size_t n_samples = 2;
  std::function<std::pair<vecP, vecP>(vecP&, vecP&, double)> line_func =
      [](vecP& sample_space, vecP samples, double threshold) -> std::pair<vecP, vecP> {
    // Slope of the line
    double x1, x2, y1, y2;
    std::tie(x1, y1) = samples.at(0);
    std::tie(x2, y2) = samples.at(1);
    assert(x1 != x2);
    double slope = (y2 - y1) / (x2 - x1);
    double intercept = y2 - (slope * x2);
    vecP inliers;
    vecP outliers;
    for (auto p : sample_space)
    {
      // std::cout << "I am here\n";
      double x, y;
      std::tie(x, y) = p;
      // Euclidean distance from a point to a line
      double dist = pointToLineDistance(p, samples.at(0), samples.at(1));
      // double y_est = (slope * x) + intercept;
      if (dist <= threshold)
      {
        inliers.emplace_back(p);
        continue;
      }
      outliers.emplace_back(p);
    }
    return { inliers, outliers };
  };
  model.compute(2, line_func, 0.4);
  return 0;
}
