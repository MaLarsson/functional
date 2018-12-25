#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace fn {
namespace detail {

template <std::size_t Index, typename Tuple, typename... Args>
constexpr decltype(auto) invoke_index(Tuple&& tuple, Args&&... args) {
    if constexpr (Index == 0) {
        return std::invoke(std::get<Index>(std::forward<Tuple>(tuple)),
                           std::forward<Args>(args)...);
    } else {
        return std::invoke(
            std::get<Index>(std::forward<Tuple>(tuple)),
            invoke_index<Index - 1>(std::forward<Tuple>(tuple),
                                    std::forward<Args>(args)...));
    }
}

}  // namespace detail

template <typename... Fs>
constexpr auto chain(Fs&&... fs) {
    return [fs = std::make_tuple(std::forward<Fs>(fs)...)](
               auto&&... args) -> decltype(auto) {
        return detail::invoke_index<std::tuple_size_v<decltype(fs)> - 1>(
            std::move(fs), std::forward<decltype(args)>(args)...);
    };
}

}  // namespace fn
