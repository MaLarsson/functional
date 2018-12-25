#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace fn {
namespace detail {

template <std::size_t N, typename F, typename... Args>
constexpr decltype(auto) curry_or_invoke(F&& f, Args&&... args);

template <std::size_t N, typename F, typename Tuple, std::size_t... I,
          typename... Args>
constexpr decltype(auto) recursive_curry(F&& f, Tuple&& tuple,
                                         std::index_sequence<I...>,
                                         Args&&... args) {
    return curry_or_invoke<N>(std::move(f),
                              std::get<I>(std::forward<Tuple>(tuple))...,
                              std::forward<decltype(args)>(args)...);
}

template <std::size_t N, typename F, typename... Args>
constexpr decltype(auto) curry_or_invoke(F&& f, Args&&... args) {
    if constexpr (sizeof...(Args) == N) {
        return std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
    } else {
        return [args = std::make_tuple(std::forward<Args>(args)...),
                f = std::forward<F>(f)](auto&&... new_args) -> decltype(auto) {
            return recursive_curry<N>(
                std::move(f), std::move(args),
                std::index_sequence_for<Args...>{},
                std::forward<decltype(new_args)>(new_args)...);
        };
    }
}

}  // namespace detail

template <std::size_t N, typename F>
constexpr auto curry(F&& f) {
    if constexpr (N == 0) {
        return [f = std::forward<F>(f)]() -> decltype(auto) {
            return std::invoke(std::move(f));
        };
    } else {
        return [f = std::forward<F>(f)](auto&&... args) -> decltype(auto) {
            return detail::curry_or_invoke<N>(
                std::move(f), std::forward<decltype(args)>(args)...);
        };
    }
}

}  // namespace fn
