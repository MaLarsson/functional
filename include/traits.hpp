#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace fn {
namespace detail {

template <typename F, typename Tuple, std::size_t... I>
inline constexpr bool is_applyable_impl(std::index_sequence<I...>) {
    return std::is_invocable_v<F,
                               decltype(std::get<I>(std::declval<Tuple>()))...>;
}

}  // namespace detail

template <typename F, typename Tuple>
struct is_applyable {
    using tuple_t = std::remove_reference_t<Tuple>;
    static constexpr bool value = detail::is_applyable_impl<F, tuple_t>(
        std::make_index_sequence<std::tuple_size_v<tuple_t>>{});
};

template <typename F, typename Tuple>
inline constexpr bool is_applyable_v = is_applyable<F, Tuple>::value;

}  // namespace fn
