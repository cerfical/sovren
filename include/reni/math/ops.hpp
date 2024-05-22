#pragma once

#include "types.hpp"
#include "util.hpp"

namespace reni {

	template <math::Vec Vec, std::invocable<float, int> F>
	void apply(Vec v, F f) noexcept {
		for(int i = 0; i < v.order(); i++) {
			f(v[i], i);
		}
	}


	template <math::Vec Vec, std::invocable<float> F>
	void apply(Vec v, F f) noexcept {
		apply(v, [&f](float v, int) { f(v); });
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float, int>
	Vec transform(Vec v, F f) noexcept {
		Vec res;
		apply(v, [&f, &res](float v, int i) { res[i] = f(v, i); });
		return res;
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float>
	Vec transform(Vec v, F f) noexcept {
		return transform(v, [&f](float v, int) { return f(v); });
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float, float, int>
	Vec combine(Vec lhs, Vec rhs, F f) noexcept {
		return transform(lhs, [&rhs, &f](float l, int i) { return f(l, rhs[i], i); });
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float, float>
	Vec combine(Vec lhs, Vec rhs, F f) noexcept {
		return combine(lhs, rhs, [&f](float l, float r, int) { return f(l, r); });
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float, int>
	float fold(Vec v, F f) noexcept {
		float res = 0;
		apply(v, [&f, &res](float v, int i) { res += f(v, i); });
		return res;
	}


	template <math::Vec Vec, typename F>
		requires std::is_invocable_r_v<float, F, float>
	float fold(Vec v, F f) noexcept {
		return fold(v, [&f](float v, int) { return f(v); });
	}


	template <math::Vec Vec>
	float sum(Vec v) noexcept {
		return fold(v, [](float v) { return v; });
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
		requires (Mat::order() == Vec::order())
	Vec operator*(Vec lhs, const Mat& rhs) noexcept {
		Vec res;
		apply(rhs, [&res, &lhs](float v, int i, int j) { res[j] += lhs[i] * v; });
		return res;
	}


	template <math::Vec Vec>
	float dot(Vec lhs, Vec rhs) noexcept {
		return sum(lhs * rhs);
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



	template <math::Mat Mat, std::invocable<float, int, int> F>
	void apply(const Mat& m, F f) noexcept {
		for(int i = 0; i < m.order(); i++) {
			for(int j = 0; j < m.order(); j++) {
				f(m[i][j], i, j);
			}
		}
	}


	template <math::Mat Mat, std::invocable<float> F>
	void apply(const Mat& m, F f) noexcept {
		apply(m, [&f](float v, int, int) { f(v); });
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float, int, int>
	Mat transform(const Mat& m, F f) noexcept {
		Mat res;
		apply(m, [&f, &res](float v, int i, int j) { res[i][j] = f(v, i, j); });
		return res;
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float>
	Mat transform(const Mat& m, F f) noexcept {
		return transform(m, [&f](float v, int, int) { return f(v); });
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float, float, int, int>
	Mat combine(const Mat& lhs, const Mat& rhs, F f) noexcept {
		return transform(lhs, [&rhs, &f](float l, int i, int j) { return f(l, rhs[i][j], i, j); });
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float, float>
	Mat combine(const Mat& lhs, const Mat& rhs, F f) noexcept {
		return combine(lhs, rhs, [&f](float l, float r, int, int) { return f(l, r); });
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float, int, int>
	float fold(const Mat& m, F f) noexcept {
		float res = 0;
		apply(m, [&f, &res](float v, int i, int j) { res += f(v, i, j); });
		return res;
	}


	template <math::Mat Mat, typename F>
		requires std::is_invocable_r_v<float, F, float>
	float fold(const Mat& m, F f) noexcept {
		return fold(m, [&f](float v, int, int) { return f(v); });
	}


	template <math::Mat Mat>
	float sum(const Mat& m) noexcept {
		return fold(m, [](float v) { return v; });
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
		for(int i = 0; i < lhs.order(); i++) {
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
