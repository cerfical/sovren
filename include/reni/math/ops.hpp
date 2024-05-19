#pragma once

#include "types.hpp"
#include "util.hpp"

namespace reni {
	
	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float, float>
	Vec combine(const Vec& lhs, const Vec& rhs, F f) noexcept {
		Vec res;
		for(int i = 0; i < Vec::len(); i++) {
			res[i] = f(lhs[i], rhs[i]);
		}
		return res;
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float>
	Vec transform(const Vec& v, F f) noexcept {
		return combine(v, Vec(), [&f](float l, float) { return f(l); });
	}



	template <math::Vec Vec>
	Vec operator+(Vec lhs, Vec rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l + r; });
	}

	template <math::Vec Vec>
	Vec operator-(Vec lhs, Vec rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l - r; });
	}

	template <math::Vec Vec>
	Vec operator*(Vec lhs, Vec rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l * r; });
	}

	template <math::Vec Vec>
	Vec operator/(Vec lhs, Vec rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l / r; });
	}



	template <math::Vec Vec>
	Vec operator+(Vec lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l + rhs; });
	}

	template <math::Vec Vec>
	Vec operator-(Vec lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l - rhs; });
	}

	template <math::Vec Vec>
	Vec operator*(Vec lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l * rhs; });
	}

	template <math::Vec Vec>
	Vec operator/(Vec lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l / rhs; });
	}



	template <math::Vec Vec, math::Mat Mat>
		requires (Mat::rowCount() == Vec::len())
	Vec operator*(Vec lhs, const Mat& rhs) noexcept {
		Vec res;
		for(int i = 0; i < Mat::colCount(); i++) {
			for(int j = 0; j < Mat::rowCount(); j++) {
				res[i] += lhs[j] * rhs[j][i];
			}
		}
		return res;
	}


	template <math::Vec Vec>
	float dot(Vec lhs, Vec rhs) noexcept {
		float res = 0;
		for(int i = 0; i < Vec::len(); i++) {
			res += lhs[i] * rhs[i];
		}
		return res;
	}



	Vec4& Vec4::operator+=(Vec4 rhs) noexcept { return *this = *this + rhs; }

	Vec4& Vec4::operator-=(Vec4 rhs) noexcept { return *this = *this - rhs; }

	Vec4& Vec4::operator*=(Vec4 rhs) noexcept { return *this = *this * rhs; }

	Vec4& Vec4::operator/=(Vec4 rhs) noexcept { return *this = *this / rhs; }


	Vec4& Vec4::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Vec4& Vec4::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Vec4& Vec4::operator*=(float rhs) noexcept { return *this = *this * rhs; }
		
	Vec4& Vec4::operator/=(float rhs) noexcept { return *this = *this / rhs; }


	Vec4& Vec4::operator*=(const Mat4x4& rhs) noexcept { return *this = *this * rhs; }


	Vec3& Vec3::operator+=(Vec3 rhs) noexcept { return *this = *this + rhs; }

	Vec3& Vec3::operator-=(Vec3 rhs) noexcept { return *this = *this - rhs; }

	Vec3& Vec3::operator*=(Vec3 rhs) noexcept { return *this = *this * rhs; }

	Vec3& Vec3::operator/=(Vec3 rhs) noexcept { return *this = *this / rhs; }


	Vec3& Vec3::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Vec3& Vec3::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Vec3& Vec3::operator*=(float rhs) noexcept { return *this = *this * rhs; }
		
	Vec3& Vec3::operator/=(float rhs) noexcept { return *this = *this / rhs; }


	Vec3& Vec3::operator*=(const Mat3x3& rhs) noexcept { return *this = *this * rhs; }


	Vec2& Vec2::operator+=(Vec2 rhs) noexcept { return *this = *this + rhs; }

	Vec2& Vec2::operator-=(Vec2 rhs) noexcept { return *this = *this - rhs; }

	Vec2& Vec2::operator*=(Vec2 rhs) noexcept { return *this = *this * rhs; }

	Vec2& Vec2::operator/=(Vec2 rhs) noexcept { return *this = *this / rhs; }


	Vec2& Vec2::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Vec2& Vec2::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Vec2& Vec2::operator*=(float rhs) noexcept { return *this = *this * rhs; }
		
	Vec2& Vec2::operator/=(float rhs) noexcept { return *this = *this / rhs; }


	Vec2& Vec2::operator*=(const Mat2x2& rhs) noexcept { return *this = *this * rhs; }



	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float, float>
	Mat combine(const Mat& lhs, const Mat& rhs, F f) noexcept {
		Mat res;
		for(int i = 0; i < Mat::rowCount(); i++) {
			for(int j = 0; j < Mat::colCount(); j++) {
				res[i][j] = f(lhs[i][j], rhs[i][j]);
			}
		}
		return res;
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float>
	Mat transform(const Mat& m, F f) noexcept {
		return combine(m, Mat(), [&f](float l, float) { return f(l); });
	}



	template <math::Mat Mat>
	Mat operator+(const Mat& lhs, const Mat& rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l + r; });
	}

	template <math::Mat Mat>
	Mat operator-(const Mat& lhs, const Mat& rhs) noexcept {
		return combine(lhs, rhs, [](float l, float r) { return l - r; });
	}

	template <math::Mat Mat>
	Mat operator*(const Mat& lhs, const Mat& rhs) noexcept {
		Mat res;
		for(int i = 0; i < Mat::rowCount(); i++) {
			res[i] = lhs[i] * rhs;
		}
		return res;
	}



	template <math::Mat Mat>
	Mat operator+(const Mat& lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l + rhs; });
	}

	template <math::Mat Mat>
	Mat operator-(const Mat& lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l - rhs; });
	}

	template <math::Mat Mat>
	Mat operator*(const Mat& lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l * rhs; });
	}
	
	template <math::Mat Mat>
	Mat operator/(const Mat& lhs, float rhs) noexcept {
		return transform(lhs, [rhs](float l) { return l / rhs; });
	}



	Mat4x4& Mat4x4::operator+=(const Mat4x4& rhs) noexcept { return *this = *this + rhs; }

	Mat4x4& Mat4x4::operator-=(const Mat4x4& rhs) noexcept { return *this = *this - rhs; }
	
	Mat4x4& Mat4x4::operator*=(const Mat4x4& rhs) noexcept { return *this = *this * rhs; }


	Mat4x4& Mat4x4::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Mat4x4& Mat4x4::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Mat4x4& Mat4x4::operator*=(float rhs) noexcept { return *this = *this * rhs; }
		
	Mat4x4& Mat4x4::operator/=(float rhs) noexcept { return *this = *this / rhs; }


	Mat3x3& Mat3x3::operator+=(const Mat3x3& rhs) noexcept { return *this = *this + rhs; }

	Mat3x3& Mat3x3::operator-=(const Mat3x3& rhs) noexcept { return *this = *this - rhs; }

	Mat3x3& Mat3x3::operator*=(const Mat3x3& rhs) noexcept { return *this = *this * rhs; }


	Mat3x3& Mat3x3::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Mat3x3& Mat3x3::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Mat3x3& Mat3x3::operator*=(float rhs) noexcept { return *this = *this * rhs; }

	Mat3x3& Mat3x3::operator/=(float rhs) noexcept { return *this = *this / rhs; }


	Mat2x2& Mat2x2::operator+=(const Mat2x2& rhs) noexcept { return *this = *this + rhs; }

	Mat2x2& Mat2x2::operator-=(const Mat2x2& rhs) noexcept { return *this = *this - rhs; }

	Mat2x2& Mat2x2::operator*=(const Mat2x2& rhs) noexcept { return *this = *this * rhs; }


	Mat2x2& Mat2x2::operator+=(float rhs) noexcept { return *this = *this + rhs; }

	Mat2x2& Mat2x2::operator-=(float rhs) noexcept { return *this = *this - rhs; }

	Mat2x2& Mat2x2::operator*=(float rhs) noexcept { return *this = *this * rhs; }

	Mat2x2& Mat2x2::operator/=(float rhs) noexcept { return *this = *this / rhs; }
}
