#pragma once

template <typename... F>
struct OverloadSet : public F... {
    OverloadSet(F... args)
        : F(args)... {}

    using F::operator()...;
};


template <typename... F>
OverloadSet<F...> overload(F... args) {
    return OverloadSet<F...>(args...);
}
