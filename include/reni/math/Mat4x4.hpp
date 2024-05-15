#pragma once

#include "Vec4.hpp"
#include <cstddef>

namespace reni {

	struct Mat4x4 {

		friend bool operator==(const Mat4x4&, const Mat4x4&) noexcept = default;


		Mat4x4() noexcept = default;

		Mat4x4(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44
		) noexcept
			: Mat4x4(
				{ _11, _12, _13, _14 },
				{ _21, _22, _23, _24 },
				{ _31, _32, _33, _34 },
				{ _41, _42, _43, _44 }
			) {}

		Mat4x4(Vec4 r1, Vec4 r2, Vec4 r3, Vec4 r4) noexcept
			: r1(r1), r2(r2), r3(r3), r4(r4) {}


		inline const Vec4& operator[](std::size_t i) const noexcept;

		inline Vec4& operator[](std::size_t i) noexcept;


		inline Mat4x4& operator+=(const Mat4x4& rhs) noexcept;
		
		inline Mat4x4& operator-=(const Mat4x4& rhs) noexcept;


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