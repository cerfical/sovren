#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

namespace reni::math {

    /**
     * @brief Describes the general concept of a mathematical vector.
     */
    template <typename T>
    concept Vec = requires(std::decay_t<T> vec, int i, float v) {
        { std::decay_t<T>::Order } -> std::convertible_to<int>;

        { std::as_const(vec)[i] } noexcept -> std::convertible_to<float>;
        { vec[i] } noexcept -> std::convertible_to<float>;

        { vec[i] = v } noexcept;
    };


    template <Vec V, std::invocable<float, int> F>
    void apply(V v, F f) noexcept {
        for(int i = 0; i < V::Order; i++) {
            f(v[i], i);
        }
    }


    template <Vec V, std::invocable<float> F>
    void apply(V v, F f) noexcept {
        apply(v, [&f](float v, int) { f(v); });
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float, int>
    V transform(V v, F f) noexcept {
        V res;
        apply(v, [&f, &res](float v, int i) { res[i] = f(v, i); });
        return res;
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float>
    V transform(V v, F f) noexcept {
        return transform(v, [&f](float v, int) { return f(v); });
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float, float, int>
    V combine(V lhs, V rhs, F f) noexcept {
        return transform(lhs, [&rhs, &f](float l, int i) { return f(l, rhs[i], i); });
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float, float>
    V combine(V lhs, V rhs, F f) noexcept {
        return combine(lhs, rhs, [&f](float l, float r, int) { return f(l, r); });
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float, int>
    float fold(V v, F f) noexcept {
        float res = 0.0f;
        apply(v, [&f, &res](float v, int i) { res += f(v, i); });
        return res;
    }


    template <Vec V, typename F>
        requires std::is_invocable_r_v<float, F, float>
    float fold(V v, F f) noexcept {
        return fold(v, [&f](float v, int) { return f(v); });
    }


    template <Vec V>
    float sum(V v) noexcept {
        return fold(v, [](float v) { return v; });
    }

}
