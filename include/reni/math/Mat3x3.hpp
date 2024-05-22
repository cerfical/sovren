#pragma once

#include "Vec3.hpp"
#include "Vec2.hpp"

#include <array>

namespace reni {

	struct Mat3x3 {

		friend bool operator==(const Mat3x3&, const Mat3x3&) noexcept = default;


		static consteval int order() noexcept {
			return static_cast<int>(decltype(r)().size());
		}


		static Mat3x3 identity() noexcept {
			return Mat3x3(
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 1.0f
			);
		}
		

		static Mat3x3 translation(Vec2 dr) noexcept {
			return Mat3x3(
				1.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				dr.x, dr.y, 1.0f
			);
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
			: r({ r1, r2, r3 }) {}


		inline const Vec3& operator[](int i) const noexcept;

		inline Vec3& operator[](int i) noexcept;


		inline Mat3x3& operator+=(const Mat3x3& rhs) noexcept;
		
		inline Mat3x3& operator-=(const Mat3x3& rhs) noexcept;

		inline Mat3x3& operator*=(const Mat3x3& rhs) noexcept;


		inline Mat3x3& operator+=(float rhs) noexcept;
		
		inline Mat3x3& operator-=(float rhs) noexcept;
		
		inline Mat3x3& operator*=(float rhs) noexcept;
		
		inline Mat3x3& operator/=(float rhs) noexcept;


		Mat3x3 transpose() const noexcept;

		Mat3x3 inverse() const noexcept;

		float determinant() const noexcept;


		std::array<Vec3, 3> r;
	};



	inline const Vec3* begin(const Mat3x3& m) noexcept {
		return m.r.data();
	}

	inline Vec3* begin(Mat3x3& m) noexcept {
		return m.r.data();
	}


	inline const Vec3* end(const Mat3x3& m) noexcept {
		return m.r.data() + m.r.size();
	}

	inline Vec3* end(Mat3x3& m) noexcept {
		return m.r.data() + m.r.size();
	}


	const Vec3& Mat3x3::operator[](int i) const noexcept {
		return begin(*this)[i];
	}

	Vec3& Mat3x3::operator[](int i) noexcept {
		return begin(*this)[i];
	}

}