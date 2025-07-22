#include <iostream>

#include <refed/defer.hpp>

int main() {
    using refed::Defer;

    //NOTE Defer objects cannot be rvalues. They need to be lvalues.
    //NOTE In c++26 you can just use the `_` underscore placeholder instead of these dummy variable names
    Defer d1{ [](){ std::cout << "It Works 1!\n"; } };
    Defer d2{ [](){ std::cout << "It Works 2!\n"; } };
    Defer d3{ [](){ std::cout << "It Works 3!\n"; } };
}
