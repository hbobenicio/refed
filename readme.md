# refed

Yet another defer c++ library, but a sane one.

Because RAII with structs/classes with dozens of lines with rules of `rand(time(nullptr))` is just complex overkill
for the majority of the cases where you just want to defer the calling of a cleanup function in a specific scope block.

Because creating dummy Deleter Functors and using std::unique_ptr just to cleanup block-scoped resources are nonsense boilerplate.

Because it's so trivial and you're tired of typing it everytime and makes you wonder why this is not on the standard library already.

Because you don't like unmaintainable and nonunderstandable macro magics for just something dumb as Defer.

Because it's dumb to rely on compiler specific macros for this simple use case.

## Usage

```cpp
#include <iostream>

#include <refed/defer.hpp>

int main() {
    using refed::Defer;

    Defer d1{ [](){ std::cout << "It Works 1!\n"; } };
    Defer d2{ [](){ std::cout << "It Works 2!\n"; } };
    Defer d3{ [](){ std::cout << "It Works 3!\n"; } };
}
```

## TODO CMake docs (install, fetchcontent dependency, plain-old tarballs, etc)
