# refed

Yet another defer c++ library, but a sane one.

Because dozens of lines for RAII with structs/classes that follows the "Rule of `rand(time(nullptr))`" is just
complex overkill for the majority of the cases where you just want to defer the calling of a
cleanup function in a specific scope block.

Because creating dummy Deleter Functors and using std::unique_ptr just to cleanup block-scoped resources are nonsense boilerplate.

Because it's so trivial and you're tired of typing it everytime and makes you wonder why this is not on the standard library already.

Because you don't like unmaintainable and nonunderstandable macro magics for just something dumb as Defer.

Because it's dumb to rely on compiler specific macros for this simple use case.

## Usage

```cpp
//NOTE just for std::cout on this example. Not needed for refer::Defer
#include <iostream>

#include <refed/defer.hpp>

int main() {
    using refed::Defer;

    //NOTE Defer objects need to be lvalues!
    //     (rvalue objects have temporary storage tied to their expressions and don't live long enough)
    //NOTE In c++26 you can just use the `_` underscore placeholder instead of these dummy variable names
    Defer d1{ [](){ std::cout << "It Works 1!\n"; } };
    Defer d2{ [](){ std::cout << "It Works 2!\n"; } };
    Defer d3{ [](){ std::cout << "It Works 3!\n"; } };

    // It will print:
    // It Works 3!
    // It Works 2!
    // It Works 1!
}
```

## Developing

### Tests

```bash
# Configuring for tests
cmake -S . -B build -DREFED_TESTS_ENABLE=ON

# Building for tests
cmake --build build

# Running tests
ctest --test-dir build/tests/unit/ --output-on-failure

# Alternative way to run tests
cd build/tests/unit/
ctest --output-on-failure

# or maybe
cd build/tests/unit/
./refed-tests-unit
```

## TODO CMake docs (install, fetchcontent dependency, plain-old tarballs, etc)
