#pragma once

#include <cmath>
#include <compare>
#include <iterator>

namespace reni::math {

    template <typename Vec, typename Mat>
    struct VecBase {

        friend std::partial_ordering operator<=>(VecBase, VecBase) noexcept = default;


        friend const float* begin(const VecBase& v) noexcept {
            return begin(const_cast<VecBase&>(v));
        }

        friend float* begin(VecBase<Vec, Mat>& v) noexcept {
            return &v.asVec()[0];
        }


        friend const float* end(const VecBase& v) noexcept {
            return end(const_cast<VecBase&>(v));
        }

        friend float* end(VecBase& v) noexcept {
            return std::next(&v.asVec()[Vec::Order - 1]);
        }


        static Vec splat(float value) noexcept {
            Vec vec;
            for(auto& col : vec) {
                col = value;
            }
            return vec;
        }


        float dot(Vec rhs) const noexcept {
            return sum(asVec() * rhs);
        }


        Vec& operator*=(const Mat& rhs) noexcept {
            return asVec() = asVec() * rhs;
        }


        Vec& operator+=(Vec rhs) noexcept {
            return asVec() = asVec() + rhs;
        }

        Vec& operator-=(Vec rhs) noexcept {
            return asVec() = asVec() - rhs;
        }

        Vec& operator*=(Vec rhs) noexcept {
            return asVec() = asVec() * rhs;
        }

        Vec& operator/=(Vec rhs) noexcept {
            return asVec() = asVec() / rhs;
        }


        Vec& operator+=(float rhs) noexcept {
            return asVec() = asVec() + rhs;
        }

        Vec& operator-=(float rhs) noexcept {
            return asVec() = asVec() - rhs;
        }

        Vec& operator*=(float rhs) noexcept {
            return asVec() = asVec() * rhs;
        }

        Vec& operator/=(float rhs) noexcept {
            return asVec() = asVec() / rhs;
        }


    private:
        const Vec& asVec() const noexcept {
            return const_cast<VecBase&>(*this).asVec();
        }

        Vec& asVec() noexcept {
            return static_cast<Vec&>(*this);
        }
    };

}
