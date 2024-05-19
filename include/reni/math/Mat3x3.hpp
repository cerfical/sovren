#pragma once

#include "Vec3.hpp"
#include "Vec2.hpp"

#include <cstddef>

namespace reni {

	struct Mat3x3 {

		friend bool operator==(const Mat3x3&, const Mat3x3&) noexcept = default;


		static Mat3x3 identity() noexcept {
			return Mat3x3(
				1, 0, 0,
				0, 1, 0,
				0, 0, 1
			);
		}
		

		static Mat3x3 translation(Vec2 ds) noexcept {
			return translation(ds.x, ds.y);
		}

		static Mat3x3 translation(float dx, float dy) noexcept {
			return Mat3x3(
				1,  0,  0,
				0,  1,  0,
				dx, dy, 1
			);
		}


		static consteval int rowCount() noexcept {
			return 3;
		}

		static consteval int colCount() noexcept {
			return 3;
		}


		Mat3x3() noexcept = default;

		Mat3x3(
			float x1, float y1, float z1,
			float x2, float y2, float z2,
			float x3, float y3, float z3
		) noexcept
			: Mat3x3(
				Vec3(x1, y1, z1),
				Vec3(x2, y2, z2),
				Vec3(x3, y3, z3)
			) {}

		Mat3x3(Vec3 r1, Vec3 r2, Vec3 r3) noexcept
			: r1(r1), r2(r2), r3(r3) {}


		inline const Vec3& operator[](int i) const noexcept;

		inline Vec3& operator[](int i) noexcept;


		inline Mat3x3& operator+=(const Mat3x3& rhs) noexcept;
		
		inline Mat3x3& operator-=(const Mat3x3& rhs) noexcept;

		inline Mat3x3& operator*=(const Mat3x3& rhs) noexcept;


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


	const Vec3& Mat3x3::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	Vec3& Mat3x3::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}