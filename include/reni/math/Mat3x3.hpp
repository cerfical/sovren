#pragma once

#include "Vec3.hpp"
#include <cstddef>

namespace reni {

	struct Mat3x3 {

		friend bool operator==(const Mat3x3&, const Mat3x3&) noexcept = default;


		Mat3x3() noexcept = default;

		Mat3x3(
			float _11, float _12, float _13,
			float _21, float _22, float _23,
			float _31, float _32, float _33
		) noexcept
			: Mat3x3(
				{ _11, _12, _13 },
				{ _21, _22, _23 },
				{ _31, _32, _33 }
			) {}

		Mat3x3(Vec3 r1, Vec3 r2, Vec3 r3) noexcept
			: r1(r1), r2(r2), r3(r3) {}


		inline const Vec3& operator[](std::size_t i) const noexcept;

		inline Vec3& operator[](std::size_t i) noexcept;


		inline Mat3x3& operator+=(const Mat3x3& rhs) noexcept;
		
		inline Mat3x3& operator-=(const Mat3x3& rhs) noexcept;


		Vec3 r1;
		Vec3 r2;
		Vec3 r3;
	};



	inline const Vec3* begin(const Mat3x3& m) noexcept {
		return &m.r1;
	}

	inline Vec3* begin(Mat3x3& m) noexcept {
		return &m.r1;
	}


	inline const Vec3* end(const Mat3x3& m) noexcept {
		return &m.r3 + 1;
	}

	inline Vec3* end(Mat3x3& m) noexcept {
		return &m.r3 + 1;
	}


	const Vec3& Mat3x3::operator[](std::size_t i) const noexcept {
		return begin(*this)[i];
	}

	Vec3& Mat3x3::operator[](std::size_t i) noexcept {
		return begin(*this)[i];
	}

}