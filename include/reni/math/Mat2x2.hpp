#pragma once

#include "Vec2.hpp"
#include <array>

namespace reni {

	struct Mat2x2 {

		friend bool operator==(const Mat2x2&, const Mat2x2&) noexcept = default;


		static consteval int order() noexcept {
			return static_cast<int>(decltype(r)().size());
		}


		static Mat2x2 identity() noexcept {
			return Mat2x2(1.0f, 0.0f, 0.0f, 1.0f);
		}


		Mat2x2() noexcept = default;

		Mat2x2(float x1, float y1, float x2, float y2) noexcept
			: Mat2x2(Vec2(x1, y1), Vec2(x2, y2)) {}

		Mat2x2(Vec2 r1, Vec2 r2) noexcept
			: r({ r1, r2 }) {}


		inline const Vec2& operator[](int i) const noexcept;

		inline Vec2& operator[](int i) noexcept;


		inline Mat2x2& operator+=(const Mat2x2& rhs) noexcept;
		
		inline Mat2x2& operator-=(const Mat2x2& rhs) noexcept;

		inline Mat2x2& operator*=(const Mat2x2& rhs) noexcept;


		inline Mat2x2& operator+=(float rhs) noexcept;
		
		inline Mat2x2& operator-=(float rhs) noexcept;
		
		inline Mat2x2& operator*=(float rhs) noexcept;
		
		inline Mat2x2& operator/=(float rhs) noexcept;


		Mat2x2 transpose() const noexcept;

		Mat2x2 inverse() const noexcept;

		float determinant() const noexcept;


		std::array<Vec2, 2> r;
	};



	inline const Vec2* begin(const Mat2x2& m) noexcept {
		return m.r.data();
	}

	inline Vec2* begin(Mat2x2& m) noexcept {
		return m.r.data();
	}


	inline const Vec2* end(const Mat2x2& m) noexcept {
		return m.r.data() + m.r.size();
	}

	inline Vec2* end(Mat2x2& m) noexcept {
		return m.r.data() + m.r.size();
	}


	const Vec2& Mat2x2::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	Vec2& Mat2x2::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}