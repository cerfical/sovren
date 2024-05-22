#pragma once

#include <utility>
#include <concepts>
#include <numbers>

namespace reni::math {
	
	/**
	 * @brief Describes the general notion of a mathematical vector.
	*/
	template <typename T>
	concept Vec = requires(std::decay_t<T> vec, int i, float v) {
		{ std::decay_t<T>::order() } noexcept -> std::convertible_to<int>;
		
		{ std::as_const(vec)[i] } noexcept -> std::convertible_to<float>;
		{ vec[i] } noexcept -> std::convertible_to<float>;

		{ vec[i] = v } noexcept;
	};


	/**
	 * @brief Describes the general notion of a linear algebra matrix.
	*/
	template <typename T>
	concept Mat = requires(std::decay_t<T> mat, int i, std::decay_t<decltype(mat[i])> vec) {
		{ std::decay_t<T>::order() } noexcept -> std::convertible_to<int>;

		{ std::as_const(mat)[i] } noexcept -> Vec;
		{ mat[i] } noexcept -> Vec;

		{ mat[i] = vec } noexcept;
	};


	/**
	 * @brief Mathematical constant Pi.
	*/
	constexpr float Pi = std::numbers::pi_v<float>;

	
	/**
	 * @brief Convert angle units from degrees to radians.
	*/
	constexpr float degToRad(float degrees) noexcept {
		return degrees * (Pi / 180.0f);
	}


	/**
	 * @brief Convert angle units from radians to degrees.
	*/
	constexpr float radToDeg(float radians) noexcept {
		return radians * (180.0f / Pi);
	}

}