#pragma once
#include <cassert>
#include <stdexcept>

namespace types
{
struct Pose2D
{
  double x;
  double y;
  [[nodiscard]] static constexpr auto dim() -> size_t
  {
    return 2;
  }
  constexpr auto operator[](size_t const& index) -> double
  {
    assert(index < dim());  // only in debug build -> Remember!
    if (index >= dim())
    {
      throw std::out_of_range("Pose2D: index out of range!");
    }
    switch (index)
    {
      case 0:
        return x;
      case 1:
        return y;
      default:
        __builtin_unreachable();
    }
  }
};
struct Pose3D
{
  double x;
  double y;
  double z;
  [[nodiscard]] static constexpr auto dim() -> size_t
  {
    return 3;
  }
  constexpr auto operator[](size_t const& index) -> double
  {
    assert(index < dim());  // only in debug build -> Remember!
    if (index >= dim())
    {
      throw std::out_of_range("Pose3D: index out of range!");
    }
    switch (index)
    {
      case 0:
        return x;
      case 1:
        return y;
      case 2:
        return z;
      default:
        __builtin_unreachable();
    }
  }
};
}  // namespace types
