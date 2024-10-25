#pragma once

#include "Mat.hpp"
#include "Vec.hpp"

#include <cmath>
#include <compare>

namespace sovren {

    template <Vec V>
    auto begin(V& vec) noexcept -> float* {
        return &vec[0];
    }

    template <Vec V>
    auto begin(const V& vec) noexcept -> const float* {
        return begin(const_cast<V&>(vec));
    }


    template <Vec V>
    auto end(V& vec) noexcept -> float* {
        return &vec[V::order() - 1] + 1;
    }

    template <Vec V>
    auto end(const V& vec) noexcept -> const float* {
        return end(const_cast<V&>(vec));
    }


    template <Vec V>
    auto fillVector(float value) noexcept -> V {
        auto vec = V();
        for(auto& c : vec) {
            c = value;
        }
        return vec;
    }


    template <Vec V>
    auto combine(V lhs, V rhs, auto f) noexcept -> V {
        for(int i = 0; i < V::order(); i++) {
            lhs[i] = f(lhs[i], rhs[i]);
        }
        return lhs;
    }

    template <Vec V>
    auto transform(V vec, auto f) noexcept -> V {
        for(auto& c : vec) {
            c = f(c);
        }
        return vec;
    }


    template <Vec V>
    auto operator<=>(V lhs, V rhs) noexcept -> std::partial_ordering {
        for(int i = 0; i < V::order(); i++) {
            if(const auto res = lhs[i] <=> rhs[i]; res != 0) {
                return res;
            }
        }
        return std::partial_ordering::equivalent;
    }

    template <Vec V>
    auto operator==(V lhs, V rhs) noexcept -> bool {
        return (lhs <=> rhs) == 0;
    }


    template <Vec V>
    auto operator+(V lhs, V rhs) noexcept -> V {
        return combine(lhs, rhs, [](auto l, auto r) { return l + r; });
    }

    template <Vec V>
    auto operator-(V lhs, V rhs) noexcept -> V {
        return combine(lhs, rhs, [](auto l, auto r) { return l - r; });
    }

    template <Vec V>
    auto operator*(V lhs, V rhs) noexcept -> V {
        return combine(lhs, rhs, [](auto l, auto r) { return l * r; });
    }

    template <Vec V>
    auto operator/(V lhs, V rhs) noexcept -> V {
        return combine(lhs, rhs, [](auto l, auto r) { return l / r; });
    }


    template <Vec V>
    auto operator+(V lhs, float rhs) noexcept -> V {
        return transform(lhs, [rhs](auto l) { return l + rhs; });
    }

    template <Vec V>
    auto operator-(V lhs, float rhs) noexcept -> V {
        return transform(lhs, [rhs](auto l) { return l - rhs; });
    }

    template <Vec V>
    auto operator*(V lhs, float rhs) noexcept -> V {
        return transform(lhs, [rhs](auto l) { return l * rhs; });
    }

    template <Vec V>
    auto operator/(V lhs, float rhs) noexcept -> V {
        return transform(lhs, [rhs](auto l) { return l / rhs; });
    }


    template <Vec V>
    auto operator+=(V& lhs, V rhs) noexcept -> V& {
        return lhs = lhs + rhs;
    }

    template <Vec V>
    auto operator-=(V& lhs, V rhs) noexcept -> V& {
        return lhs = lhs - rhs;
    }

    template <Vec V>
    auto operator*=(V& lhs, V rhs) noexcept -> V& {
        return lhs = lhs * rhs;
    }

    template <Vec V>
    auto operator/=(V& lhs, V rhs) noexcept -> V& {
        return lhs = lhs / rhs;
    }


    template <Vec V>
    auto operator+=(V& lhs, float rhs) noexcept -> V& {
        return lhs = lhs + rhs;
    }

    template <Vec V>
    auto operator-=(V& lhs, float rhs) noexcept -> V& {
        return lhs = lhs - rhs;
    }

    template <Vec V>
    auto operator*=(V& lhs, float rhs) noexcept -> V& {
        return lhs = lhs * rhs;
    }

    template <Vec V>
    auto operator/=(V& lhs, float rhs) noexcept -> V& {
        return lhs = lhs / rhs;
    }


    template <Vec V, Mat M>
    auto operator*(V lhs, const M& rhs) noexcept -> V {
        static_assert(V::order() == M::order(), "vectors are multipled by matrices of the same order");

        auto res = V();
        for(int i = 0; i < V::order(); i++) {
            res += fillVector<V>(lhs[i]) * rhs[i];
        }
        return res;
    }

    template <Vec V, Mat M>
    auto operator*=(V& lhs, const M& rhs) noexcept -> V& {
        return lhs = lhs * rhs;
    }


    template <Vec V>
    auto sum(V vec) noexcept -> float {
        float res = 0;
        for(const auto& c : vec) {
            res += c;
        }
        return res;
    }

    template <Vec V>
    auto dot(V lhs, V rhs) noexcept -> float {
        return sum(lhs * rhs);
    }

    template <Vec V>
    auto len(V vec) noexcept -> float {
        return std::sqrt(dot(vec, vec));
    }

    template <Vec V>
    auto normalize(V vec) noexcept -> V {
        return vec / len(vec);
    }


    template <Vec V>
    auto operator-(V vec) noexcept -> V {
        return V() - vec;
    }

    template <Vec V>
    auto operator+(V vec) noexcept -> V {
        return vec;
    }

}
