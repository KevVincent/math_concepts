#pragma once
#include <concepts>
#include <functional>
#include <memory>
#include <stdexcept>

namespace kd_tree
{
template <typename T>
concept PointLike = requires(T t, size_t i) {
  { T::dim() } -> std::convertible_to<std::size_t>;
  { t[i] } -> std::convertible_to<double>;
};
template <PointLike T>
struct Node
{
  std::shared_ptr<T> value;
  std::shared_ptr<Node<T>> left;
  std::shared_ptr<Node<T>> right;
};

template <PointLike T>
class KDTree
{
  using T_ptr = std::shared_ptr<T>;

public:
  /*
   * TODO: Should find the median. And make the data into a tree.
   * With nodes as shrd ptrs will I save overall memory always. Need to make sure i dont over write stuff here then.
   */
  KDTree(std::vector<T_ptr>& nodes, size_t dimension)
  {
    root_ = buildTree(nodes, dimension);
  }

  /*
   * Builds a tree. Assumption: Only called with sorted data.
   */
  [[nodiscard]] auto buildTree(std::vector<T_ptr> nodes, size_t dimension) -> std::shared_ptr<Node<T>>
  {
    if (nodes.empty())
    {
      throw std::runtime_error("KDTree Build: cannot build tree with empty data!");
    }
    // A leaf node.
    if (nodes.size() == 1)
    {
      auto n = std::make_shared<Node<T>>();
      n->value = nodes.front();
      return n;
    }
    // First the sort nodes with respect to the func.. Options: Heap sort, ??
    // sort the nodes ()
    std::sort(nodes.begin(), nodes.end(), [this, &dimension](const T_ptr& a, const T_ptr& b) {
      return (*a)[dimension] < (*b)[dimension];
    });  // Okay this is intro sort, Mutant freak kid of heap, quick
         // and insertion.
    size_t median = nodes.size() >> 1;
    auto node = std::make_shared<Node<T>>();
    node->value = nodes.at(median);
    dimension = (dimension + 1) % T::dim();
    // This makes unnecessary copy. Probably better just with ranges view. but what is the signature of build tree like?
    std::vector<T_ptr> left(nodes.begin(), nodes.begin() + median);
    node->left = buildTree(left, dimension);
    std::vector<T_ptr> right(nodes.begin() + median + 1, nodes.end());
    node->left = buildTree(right, dimension);
    return node;
  }

private:
  /*
   * Root node
   */
  std::shared_ptr<Node<T>> root_;
};
}  // namespace kd_tree
