#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace fn {
namespace detail {

template <std::size_t I, typename Tuple, typename... Args>
constexpr decltype(auto) invoke_overload(Tuple&& fs, Args&&... args) {
    using fn_sig = decltype(std::get<I>(std::forward<Tuple>(fs)));
    constexpr bool invocable =
        std::is_invocable_v<fn_sig, decltype(std::forward<Args>(args))...>;

    static_assert(I > 0 || invocable, "no matching overload");

    if constexpr (invocable) {
        return std::invoke(std::get<I>(std::forward<Tuple>(fs)),
                           std::forward<Args>(args)...);
    } else if constexpr (I > 0) {
        return invoke_overload<I - 1>(std::forward<Tuple>(fs),
                                      std::forward<Args>(args)...);
    }
}

}  // namespace detail

template <typename... Fs>
constexpr auto overload(Fs&&... fs) {
    return [fs = std::make_tuple(std::forward<Fs>(fs)...)](
               auto&&... args) -> decltype(auto) {
        return detail::invoke_overload<std::tuple_size_v<decltype(fs)> - 1>(
            fs, std::forward<decltype(args)>(args)...);
    };
}

}  // namespace fn
