#pragma once

namespace refed
{
    /**
     * Defer is a utility class that trivially implements a "defer" functionality using its destructor.
     * 
     * #### Usage
     * 
     * ```cpp
     * Defer _{[]{ std::cout << "It Works 1!\n"; }};
     * Defer _{[]{ std::cout << "It Works 2!\n"; }};
     * ```
     */
    template<class Func>
    class Defer {
    private:
        const Func func;
        
    public:
        // Let's delete all problematic copy and move constructors/assignments to avoid double calling the defered callback
        Defer(const Defer&)            = delete;  // Copy constructor
        Defer(Defer&&)                 = delete;  // Move constructor
        Defer& operator=(const Defer&) = delete;  // Copy assignment
        Defer& operator=(Defer&&)      = delete;  // Move assignment

        explicit Defer(Func func) noexcept
            : func(func)
        {}

        ~Defer() {
            func();
        }
    };
}
