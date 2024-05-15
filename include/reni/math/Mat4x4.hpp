#pragma once

#include "Vec4.hpp"
#include <cstddef>

namespace reni {

	struct Mat4x4 {

		friend bool operator==(const Mat4x4&, const Mat4x4&) noexcept = default;


		static Mat4x4 identity() noexcept {
			return Mat4x4(
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0
			);
		}


		Mat4x4() noexcept = default;

		Mat4x4(
			float x1, float y1, float z1, float w1,
			float x2, float y2, float z2, float w2,
			float x3, float y3, float z3, float w3,
			float x4, float y4, float z4, float w4
		) noexcept
			: Mat4x4(
				Vec4(x1, y1, z1, w1),
				Vec4(x2, y2, z2, w2),
				Vec4(x3, y3, z3, w3),
				Vec4(x4, y4, z4, w4)
			) {}

		Mat4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) noexcept
			: r1(r1), r2(r2), r3(r3), r4(r4) {}


		inline const Vec4& operator[](std::size_t i) const noexcept;

		inline Vec4& operator[](std::size_t i) noexcept;


		inline Mat4x4& operator+=(const Mat4x4& rhs) noexcept;
		
		inline Mat4x4& operator-=(const Mat4x4& rhs) noexcept;

		inline Mat4x4& operator*=(const Mat4x4& rhs) noexcept;


		Vec4 r1;
		Vec4 r2;
		Vec4 r3;
		Vec4 r4;
	};



	inline const Vec4* begin(const Mat4x4& m) noexcept {
		return &m.r1;
	}

	inline Vec4* begin(Mat4x4& m) noexcept {
		return &m.r1;
	}


	inline const Vec4* end(const Mat4x4& m) noexcept {
		return &m.r4 + 1;
	}

	inline Vec4* end(Mat4x4& m) noexcept {
		return &m.r4 + 1;
	}


	const Vec4& Mat4x4::operator[](std::size_t i) const noexcept {
		return begin(*this)[i];
	}

	Vec4& Mat4x4::operator[](std::size_t i) noexcept {
		return begin(*this)[i];
	}

}