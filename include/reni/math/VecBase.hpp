#pragma once

#include <cmath>
#include <compare>
#include <concepts>
#include <iterator>

namespace reni::math {

    template <typename Vec, typename Mat>
    struct VecBase {

        friend std::partial_ordering operator<=>(VecBase, VecBase) noexcept = default;


        friend Vec operator*(Vec lhs, const Mat& rhs) noexcept { return lhs *= rhs; }

        friend Vec operator+(Vec lhs, Vec rhs) noexcept { return lhs += rhs; }
        friend Vec operator-(Vec lhs, Vec rhs) noexcept { return lhs -= rhs; }
        friend Vec operator*(Vec lhs, Vec rhs) noexcept { return lhs *= rhs; }
        friend Vec operator/(Vec lhs, Vec rhs) noexcept { return lhs /= rhs; }

        friend Vec operator+(Vec lhs, float rhs) noexcept { return lhs += rhs; }
        friend Vec operator-(Vec lhs, float rhs) noexcept { return lhs -= rhs; }
        friend Vec operator*(Vec lhs, float rhs) noexcept { return lhs *= rhs; }
        friend Vec operator/(Vec lhs, float rhs) noexcept { return lhs /= rhs; }


        friend const float* begin(const Vec& v) noexcept { return begin(const_cast<Vec&>(v)); }

        friend const float* end(const Vec& v) noexcept { return end(const_cast<Vec&>(v)); }


        static Vec splat(float value) noexcept {
            return Vec().transform([value](auto) { return value; });
        }


        template <std::invocable<float, float> F>
        float fold(float v, F f) const noexcept {
            for(const auto& col : asVec()) {
                v = f(v, col);
            }
            return v;
        }


        template <std::invocable<float> F>
        Vec transform(F f) const noexcept {
            auto vec = Vec();
            for(int i = 0; i < size(); i++) {
                vec[i] = f(asVec()[i]);
            }
            return vec;
        }


        template <std::invocable<float, float> F>
        Vec combine(Vec rhs, F f) const noexcept {
            auto vec = Vec();
            for(int i = 0; i < size(); i++) {
                vec[i] = f(asVec()[i], rhs[i]);
            }
            return vec;
        }


        float dot(Vec rhs) const noexcept { return (asVec() * rhs).sum(); }

        float sum() const noexcept {
            return fold(0.0f, [](auto l, auto r) { return l + r; });
        }

        float len() const noexcept { return std::sqrt(dot(asVec())); }


        Vec normalize() const noexcept { return asVec() / len(); }

        Vec operator-() const noexcept { return Vec() - asVec(); }
        Vec operator+() const noexcept { return asVec(); }


        Vec& operator*=(const Mat& rhs) noexcept {
            auto& vec = asVec();
            const auto lhs = std::exchange(vec, Vec());

            for(int i = 0; i < lhs.size(); i++) {
                vec += splat(lhs[i]) * rhs[i];
            }

            return vec;
        }


        Vec& operator+=(Vec rhs) noexcept {
            return asVec() = combine(rhs, [](auto l, auto r) { return l + r; });
        }

        Vec& operator-=(Vec rhs) noexcept {
            return asVec() = combine(rhs, [](auto l, auto r) { return l - r; });
        }

        Vec& operator*=(Vec rhs) noexcept {
            return asVec() = combine(rhs, [](auto l, auto r) { return l * r; });
        }

        Vec& operator/=(Vec rhs) noexcept {
            return asVec() = combine(rhs, [](auto l, auto r) { return l / r; });
        }


        Vec& operator+=(float rhs) noexcept {
            return asVec() = transform([rhs](auto l) { return l + rhs; });
        }

        Vec& operator-=(float rhs) noexcept {
            return asVec() = transform([rhs](auto l) { return l - rhs; });
        }

        Vec& operator*=(float rhs) noexcept {
            return asVec() = transform([rhs](auto l) { return l * rhs; });
        }

        Vec& operator/=(float rhs) noexcept {
            return asVec() = transform([rhs](auto l) { return l / rhs; });
        }


        float operator[](int i) const noexcept { return const_cast<Vec&>(asVec())[i]; }

        float& operator[](int i) noexcept { return *std::next(begin(asVec()), i); }


        int size() const noexcept { return static_cast<int>(end(asVec()) - begin(asVec())); }


    private:
        const Vec& asVec() const noexcept { return static_cast<const Vec&>(*this); }

        Vec& asVec() noexcept { return static_cast<Vec&>(*this); }
    };

}
