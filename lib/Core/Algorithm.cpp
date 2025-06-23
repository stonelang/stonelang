#include "stone/Core/Algorithm.h"

using namespace stone;

template <typename Range, typename Predicate>
bool stone::ForEach(const Range &range, Predicate &&pred) {
  return std::any_of(range.begin(), range.end(), std::forward<Predicate>(pred));
}
