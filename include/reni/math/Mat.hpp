#pragma once

#include "Vec.hpp"

#include <concepts>
#include <type_traits>
#include <utility>

namespace reni::math {

    /**
     * @brief Describes the general concept of a linear algebra matrix.
     */
    template <typename T>
    concept Mat = requires(std::decay_t<T> mat, int i, std::decay_t<decltype(mat[i])> vec) {
        { std::decay_t<T>::Order } -> std::convertible_to<int>;

        { std::as_const(mat)[i] } noexcept -> Vec;
        { mat[i] } noexcept -> Vec;

        { mat[i] = vec } noexcept;
    };


    template <Mat M, std::invocable<float, int, int> F>
    void apply(const M& m, F f) noexcept {
        for(int i = 0; i < M::Order; i++) {
            for(int j = 0; j < M::Order; j++) {
                f(m[i][j], i, j);
            }
        }
    }


    template <Mat M, std::invocable<float> F>
    void apply(const M& m, F f) noexcept {
        apply(m, [&f](float v, int, int) { f(v); });
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float, int, int>
    M transform(const M& m, F f) noexcept {
        M res;
        apply(m, [&f, &res](float v, int i, int j) { res[i][j] = f(v, i, j); });
        return res;
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float>
    M transform(const M& m, F f) noexcept {
        return transform(m, [&f](float v, int, int) { return f(v); });
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float, float, int, int>
    M combine(const M& lhs, const M& rhs, F f) noexcept {
        return transform(lhs, [&rhs, &f](float l, int i, int j) { return f(l, rhs[i][j], i, j); });
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float, float>
    M combine(const M& lhs, const M& rhs, F f) noexcept {
        return combine(lhs, rhs, [&f](float l, float r, int, int) { return f(l, r); });
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float, int, int>
    float fold(const M& m, F f) noexcept {
        float res = 0.0f;
        apply(m, [&f, &res](float v, int i, int j) { res += f(v, i, j); });
        return res;
    }


    template <Mat M, typename F>
        requires std::is_invocable_r_v<float, F, float>
    float fold(const M& m, F f) noexcept {
        return fold(m, [&f](float v, int, int) { return f(v); });
    }


    template <Mat M>
    float sum(const M& m) noexcept {
        return fold(m, [](float v) { return v; });
    }

}
