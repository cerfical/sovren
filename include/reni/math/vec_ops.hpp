#pragma once

#include "Mat.hpp"
#include "Vec.hpp"

namespace reni {

    template <math::Vec Vec>
    Vec operator+(Vec lhs, Vec rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l + r; });
    }


    template <math::Vec Vec>
    Vec operator-(Vec lhs, Vec rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l - r; });
    }


    template <math::Vec Vec>
    Vec operator*(Vec lhs, Vec rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l * r; });
    }


    template <math::Vec Vec>
    Vec operator/(Vec lhs, Vec rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l / r; });
    }


    template <math::Vec Vec>
    Vec operator+(Vec lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l + rhs; });
    }


    template <math::Vec Vec>
    Vec operator-(Vec lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l - rhs; });
    }


    template <math::Vec Vec>
    Vec operator*(Vec lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l * rhs; });
    }


    template <math::Vec Vec>
    Vec operator/(Vec lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l / rhs; });
    }


    template <math::Vec Vec, math::Mat Mat>
        requires (Mat::Order == Vec::Order)
    Vec operator*(Vec lhs, const Mat& rhs) noexcept {
        Vec res;
        math::apply(rhs, [&res, &lhs](float v, int i, int j) { res[j] += lhs[i] * v; });
        return res;
    }

}
