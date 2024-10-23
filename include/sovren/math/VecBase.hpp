#pragma once

#include <cmath>
#include <compare>
#include <iterator>

namespace sovren::math {

    template <typename Vec, typename Mat, int Ord>
    struct VecBase {

        static inline constexpr int Order = Ord;


        friend std::partial_ordering operator<=>(Vec lhs, Vec rhs) noexcept {
            for(int i = 0; i < Order; i++) {
                if(const auto res = lhs[i] <=> rhs[i]; res != 0) {
                    return res;
                }
            }
            return std::partial_ordering::equivalent;
        }

        friend bool operator==(Vec lhs, Vec rhs) noexcept {
            return (lhs <=> rhs) == 0;
        }


        friend Vec operator*(Vec lhs, const Mat& rhs) noexcept {
            return lhs *= rhs;
        }

        friend Vec operator+(Vec lhs, Vec rhs) noexcept {
            return lhs += rhs;
        }

        friend Vec operator-(Vec lhs, Vec rhs) noexcept {
            return lhs -= rhs;
        }

        friend Vec operator*(Vec lhs, Vec rhs) noexcept {
            return lhs *= rhs;
        }

        friend Vec operator/(Vec lhs, Vec rhs) noexcept {
            return lhs /= rhs;
        }


        friend Vec operator+(Vec lhs, float rhs) noexcept {
            return lhs += rhs;
        }

        friend Vec operator-(Vec lhs, float rhs) noexcept {
            return lhs -= rhs;
        }

        friend Vec operator*(Vec lhs, float rhs) noexcept {
            return lhs *= rhs;
        }

        friend Vec operator/(Vec lhs, float rhs) noexcept {
            return lhs /= rhs;
        }


        friend Vec& operator*=(Vec& lhs, const Mat& rhs) noexcept {
            auto res = Vec();
            for(int i = 0; i < Order; i++) {
                res += splat(lhs[i]) * rhs[i];
            }
            return lhs = res;
        }


        friend Vec& operator+=(Vec& lhs, Vec rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l += r; });
        }

        friend Vec& operator-=(Vec& lhs, Vec rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l -= r; });
        }

        friend Vec& operator*=(Vec& lhs, Vec rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l *= r; });
        }

        friend Vec& operator/=(Vec& lhs, Vec rhs) noexcept {
            return lhs.combine(rhs, [](auto& l, auto r) { l /= r; });
        }


        friend Vec& operator+=(Vec& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l += rhs; });
        }

        friend Vec& operator-=(Vec& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l -= rhs; });
        }

        friend Vec& operator*=(Vec& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l *= rhs; });
        }

        friend Vec& operator/=(Vec& lhs, float rhs) noexcept {
            return lhs.transform([rhs](auto& l) { l /= rhs; });
        }


        friend const float* begin(const Vec& v) noexcept {
            return begin(const_cast<Vec&>(v));
        }

        friend const float* end(const Vec& v) noexcept {
            return end(const_cast<Vec&>(v));
        }


        static Vec splat(float value) noexcept {
            return Vec() + value;
        }


        void normalize() noexcept {
            asVec() /= len();
        }

        Vec normalized() const noexcept {
            auto vec = asVec();
            vec.normalize();
            return vec;
        }


        float dot(Vec rhs) const noexcept {
            return (asVec() * rhs).sum();
        }

        float len() const noexcept {
            return std::sqrt(dot(asVec()));
        }


        Vec operator-() const noexcept {
            return Vec() - asVec();
        }

        Vec operator+() const noexcept {
            return asVec();
        }


        const float& operator[](int i) const noexcept {
            return const_cast<Vec&>(asVec())[i];
        }

        float& operator[](int i) noexcept {
            return *std::next(begin(asVec()), i);
        }


    private:
        const Vec& asVec() const noexcept {
            return static_cast<const Vec&>(*this);
        }

        Vec& asVec() noexcept {
            return static_cast<Vec&>(*this);
        }


        float sum() const noexcept {
            float res = 0.0f;
            for(int i = 0; i < Order; i++) {
                res += (*this)[i];
            }
            return res;
        }

        Vec& transform(auto f) noexcept {
            for(int i = 0; i < Order; i++) {
                f((*this)[i]);
            }
            return asVec();
        }

        Vec& combine(Vec rhs, auto f) noexcept {
            for(int i = 0; i < Order; i++) {
                f((*this)[i], rhs[i]);
            }
            return asVec();
        }
    };

}
