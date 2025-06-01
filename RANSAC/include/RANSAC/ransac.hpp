#pragma once
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <random>
#include <utility>
#include <vector>
using Point = std::pair<double, double>;
namespace algorithms
{
template <typename T, typename ThresType>
class RANSAC
{
public:
  using vecT = std::vector<T>;
  using ModelFunction = std::function<std::pair<vecT, vecT>(vecT&, vecT&, double)>;
  RANSAC(std::vector<T> datapoints, ThresType threshold, double outlier_ratio_threshold, double success_probablity)
    : data_points_(datapoints)
    , threshold_(threshold)
    , outlier_ratio_threshold_(outlier_ratio_threshold)
    , success_probability_(success_probablity)
  {
  }
  /**
   * Creates the chosen model with the sampled data points
   */
  void compute(size_t n_samples, ModelFunction func, double cut_off_threshold)
  {
    calculateAndSetIterations(n_samples);
    size_t i = 0;
    std::cout << "The number of iterations : " << iterations_ << "\n";
    while (i < iterations_)
    {
      auto random_samples = sample(n_samples);
      double x1, x2, y1, y2;
      std::tie(x1, y1) = random_samples.at(0);
      std::tie(x2, y2) = random_samples.at(1);
      std::cout << "The line is along " << x1 << " , " << y1 << " and " << x2 << " , " << y2 << "\n";
      assert(random_samples.size() == n_samples);
      vecT inliers;
      vecT outliers;
      std::tie(inliers, outliers) = func(data_points_, random_samples, threshold_);
      std::cout << "The score is : " << score(inliers) << "\n";
      if (score(inliers) >= cut_off_threshold)
      {
        double x1, x2, y1, y2;
        std::tie(x1, y1) = random_samples.at(0);
        std::tie(x2, y2) = random_samples.at(1);
        std::cout << "The best line is along " << x1 << " , " << y1 << " and " << x2 << " , " << y2 << "\n";
        break;
      }
      ++i;
    }
  }

private:
  /**
   * Calculate the maximum number of iterations.
   */
  void calculateAndSetIterations(size_t n_samples)
  {
    // Probability that a sample is an inlier
    auto p_inlier = 1 - outlier_ratio_threshold_;
    // Probability that all samples are inliers. ( P of drawing only inliers )
    auto p_n_inliers = std::pow(p_inlier, n_samples);
    // Probability of failing once
    auto p_failure_once = 1 - p_n_inliers;
    assert(p_failure_once != 0 && "Calculated failure probability is zero. Possible division by zero");
    // Probability of failing all the iterations to pick an inlier
    // 1 - success_probability = std::pow(p_failure_once, iterations)
    // p_failure_t = std::pow(p_failure_once, iterations)
    // p_failure_once = 1 - std::pow(1-outlier_ratio_threshold, n_samples)
    // T = std::log(1-success_probability)/std::log(p_failure_once)
    iterations_ = std::ceil(std::log(1 - success_probability_) / std::log(p_failure_once));
    iterations_ = std::max<size_t>(1, iterations_);
  }
  /**
   * Samples the sub set for model fitting.
   * Number of samples are based on chosen model.
   */
  [[nodiscard]] auto sample(size_t n_samples) -> std::vector<T>
  {
    std::random_device rd;
    std::mt19937 mt(rd());
    auto point_copy = data_points_;
    std::shuffle(point_copy.begin(), point_copy.end(), mt);
    return std::vector<T>(point_copy.begin(), point_copy.begin() + n_samples);
  }

  /**
   * Computes the inliers based on the threshold
   */
  auto score(vecT inliers) -> double
  {
    return static_cast<double>(inliers.size()) / data_points_.size();
  }

  /**
   * Vector of data points (May be a stack/deque is better)
   */
  std::vector<T> data_points_;
  /**
   * Tolerance threshold based on which a point is considered an inlier.
   */
  ThresType threshold_;
  /**
   * Maximum number of iterations
   */
  size_t iterations_;
  /**
   * Ouliers to total set ratio threshold. (Used for calculation cut off)
   */
  double outlier_ratio_threshold_;
  /**
   * Probability that every drawn sample is an inlier
   */
  double success_probability_{ 0.99 };
};
}  // namespace algorithms
