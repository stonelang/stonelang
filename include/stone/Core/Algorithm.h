#ifndef STONE_CORE_ALGORITHM_H
#define STONE_CORE_ALGORITHM_H

#include <algorithm>
#include <iostream>

namespace stone {
template <typename Range, typename Predicate>
bool ForEach(const Range &range, Predicate &&pred);

// constexpr auto ForEach = [](auto&& range, auto&& pred) {
//     return std::any_of(std::begin(range), std::end(range),
//     std::forward<decltype(pred)>(pred));
// };

} // namespace stone
#endif