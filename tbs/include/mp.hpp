#include <concepts>
#include <iostream>
#include <ranges>

namespace tbs {

template <typename T>
concept arithmetic = std::integral<T> || std::floating_point<T>;

template <typename T, typename... U>
concept is_any_of = (std::same_as<T, U> || ...);

template <typename T>
concept printable = requires(T t, std::ostream &os) {
  { os << t } -> std::same_as<std::ostream &>;
};

} // namespace tbs