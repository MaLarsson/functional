#include <iostream>

#include "chain.hpp"
#include "curry.hpp"
#include "overload.hpp"

int main() {
    auto test = fn::chain([](int i, int j) { return i + j; },
                          [](int i) { return i * 2; },
                          [](int i) -> std::tuple<int, int> {
                              return {i, i};
                          },
                          [](int i, int j) { return i + j; });

    std::cout << test(1, 2) << '\n';

    auto test2 = fn::curry<3>([](int a, int b, int c) { return a + b + c; });

    std::cout << test2(1)(2)(3) << ", ";
    std::cout << test2(1, 2)(3) << ", ";
    std::cout << test2(1)(2, 3) << ", ";
    std::cout << test2(1, 2, 3) << '\n';

    auto test3 = fn::curry<0>([]() { return 42; });

    std::cout << test3() << '\n';

    auto test4 = fn::overload([](int i) { return i; },
                              [](const char* str) { return str; });

    std::cout << test4(42) << ", ";
    std::cout << test4("test") << '\n';

    auto test5 = fn::overload([](int) { return "int"; },
                              [](const char*) { return "string"; });

    std::cout << test5(42) << ", ";
    std::cout << test5("test") << '\n';

    return 0;
}
