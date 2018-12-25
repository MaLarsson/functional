#include <iostream>

#include "chain.hpp"
#include "curry.hpp"

int main() {
    auto test = fn::chain([](int i, int j) { return i + j; },
                          [](int i) { return i * 2; });

    std::cout << test(1, 2) << '\n';

    auto test2 = fn::curry<3>([](int a, int b, int c) { return a + b + c; });

    std::cout << test2(1)(2)(3) << ", ";
    std::cout << test2(1, 2)(3) << ", ";
    std::cout << test2(1)(2, 3) << ", ";
    std::cout << test2(1, 2, 3) << '\n';

    auto test3 = fn::curry<0>([]() { return 42; });

    std::cout << test3() << '\n';

    return 0;
}
