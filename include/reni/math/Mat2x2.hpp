#pragma once

#include "Vec2.hpp"
#include <cstddef>

namespace reni {

	struct Mat2x2 {

		friend bool operator==(const Mat2x2&, const Mat2x2&) noexcept = default;


		Mat2x2() noexcept = default;

		Mat2x2(
			float _11, float _12,
			float _21, float _22
		) noexcept
			: Mat2x2(
				{ _11, _12 },
				{ _21, _22 }
			) {}

		Mat2x2(Vec2 r1, Vec2 r2) noexcept
			: r1(r1), r2(r2) {}


		inline const Vec2& operator[](std::size_t i) const noexcept;

		inline Vec2& operator[](std::size_t i) noexcept;


		inline Mat2x2& operator+=(const Mat2x2& rhs) noexcept;
		
		inline Mat2x2& operator-=(const Mat2x2& rhs) noexcept;


		Vec2 r1;
		Vec2 r2;
	};



	inline const Vec2* begin(const Mat2x2& m) noexcept {
		return &m.r1;
	}

	inline Vec2* begin(Mat2x2& m) noexcept {
		return &m.r1;
	}


	inline const Vec2* end(const Mat2x2& m) noexcept {
		return &m.r2 + 1;
	}

	inline Vec2* end(Mat2x2& m) noexcept {
		return &m.r2 + 1;
	}


	const Vec2& Mat2x2::operator[](std::size_t i) const noexcept {
		return begin(*this)[i];
	}

	Vec2& Mat2x2::operator[](std::size_t i) noexcept {
		return begin(*this)[i];
	}

}