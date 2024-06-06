#pragma once

#include "Mat.hpp"

namespace reni {

    template <math::Mat Mat>
    Mat operator+(const Mat& lhs, const Mat& rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l + r; });
    }


    template <math::Mat Mat>
    Mat operator-(const Mat& lhs, const Mat& rhs) noexcept {
        return math::combine(lhs, rhs, [](float l, float r) { return l - r; });
    }


    template <math::Mat Mat>
    Mat operator*(const Mat& lhs, const Mat& rhs) noexcept {
        Mat res;
        for(int i = 0; i < Mat::Order; i++) {
            res[i] = lhs[i] * rhs;
        }
        return res;
    }


    template <math::Mat Mat>
    Mat operator+(const Mat& lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l + rhs; });
    }


    template <math::Mat Mat>
    Mat operator-(const Mat& lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l - rhs; });
    }


    template <math::Mat Mat>
    Mat operator*(const Mat& lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l * rhs; });
    }


    template <math::Mat Mat>
    Mat operator/(const Mat& lhs, float rhs) noexcept {
        return math::transform(lhs, [rhs](float l) { return l / rhs; });
    }

}
