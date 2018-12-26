#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "traits.hpp"

namespace fn {
namespace detail {

template <std::size_t I, typename Tuple, typename... Args>
constexpr decltype(auto) recurse_or_call_index(Tuple&& tuple, Args&&... args);

template <std::size_t I, typename Tuple, typename... Args>
constexpr decltype(auto) call_index(Tuple&& tuple, Args&&... args) {
    using fn_sig = decltype(std::get<I>(std::forward<Tuple>(tuple)));
    constexpr bool invocable =
        std::is_invocable_v<fn_sig, decltype(std::forward<Args>(args))...>;

    if constexpr (invocable) {
        return std::invoke(std::get<I>(std::forward<Tuple>(tuple)),
                           std::forward<Args>(args)...);
    } else {
        constexpr bool applyable =
            fn::is_applyable_v<fn_sig, decltype(std::forward<Args>(args))...>;

        static_assert(applyable, "fn::chain functions does not match");

        return std::apply(std::get<I>(std::forward<Tuple>(tuple)),
                          std::forward<Args>(args)...);
    }
}

template <std::size_t I, typename Tuple, typename... Args>
constexpr decltype(auto) recurse_index(Tuple&& tuple, Args&&... args) {
    using fn_sig = decltype(std::get<I>(std::forward<Tuple>(tuple)));
    constexpr bool invocable =
        std::is_invocable_v<fn_sig, decltype(recurse_or_call_index<I - 1>(
                                        std::forward<Tuple>(tuple),
                                        std::forward<Args>(args)...))>;

    if constexpr (invocable) {
        return std::invoke(
            std::get<I>(std::forward<Tuple>(tuple)),
            recurse_or_call_index<I - 1>(std::forward<Tuple>(tuple),
                                         std::forward<Args>(args)...));
    } else {
        constexpr bool applyable =
            fn::is_applyable_v<fn_sig, decltype(recurse_or_call_index<I - 1>(
                                           std::forward<Tuple>(tuple),
                                           std::forward<Args>(args)...))>;

        static_assert(applyable, "fn::chain functions does not match");

        return std::apply(
            std::get<I>(std::forward<Tuple>(tuple)),
            recurse_or_call_index<I - 1>(std::forward<Tuple>(tuple),
                                         std::forward<Args>(args)...));
    }
}

template <std::size_t I, typename Tuple, typename... Args>
constexpr decltype(auto) recurse_or_call_index(Tuple&& tuple, Args&&... args) {
    if constexpr (I == 0) {
        return call_index<I>(std::forward<Tuple>(tuple),
                             std::forward<Args>(args)...);
    } else {
        return recurse_index<I>(std::forward<Tuple>(tuple),
                                std::forward<Args>(args)...);
    }
}

}  // namespace detail

template <typename... Fs>
constexpr auto chain(Fs&&... fs) {
    return [fs = std::make_tuple(std::forward<Fs>(fs)...)](
               auto&&... args) -> decltype(auto) {
        constexpr std::size_t tuple_size = std::tuple_size_v<decltype(fs)>;
        return detail::recurse_or_call_index<tuple_size - 1>(
            fs, std::forward<decltype(args)>(args)...);
    };
}

}  // namespace fn
