#pragma once

namespace refed
{
    /**
     * Defer is a utility class that trivially implements a "defer" functionality using its destructor
     */
    template<class Func>
    class Defer {
    private:
        const Func func;
        
    public:
        // Let's delete all problematic copy and move constructors/assignments
        // to avoid calling the defered callback more than once
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
