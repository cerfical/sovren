#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Vec4 : public testing::Test {
protected:
	const reni::Vec4 v1 = { 1.0, 2.0, 3.0, 4.0 };
	const reni::Vec4 v2 = { 2.0, 4.0, 6.0, 8.0 };
	
	const reni::Mat4x4 m1 = {
		2.0, 0.0, 0.0, 0.0,
		0.0, 2.0, 0.0, 0.0,
		0.0, 0.0, 2.0, 0.0,
		0.0, 0.0, 0.0, 2.0
	};

	reni::Vec4 v;
};

TEST_F(Vec4, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 4.0);
}

TEST_F(Vec4, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 4.0);
	EXPECT_EQ(v, v1);
}

TEST_F(Vec4, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(Vec4, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(Vec4, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(Vec4, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(Vec4, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(Vec4, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 60.0);
}

TEST_F(Vec4, MatrixMultiplication) {
	ASSERT_EQ(v1 * m1, v2);
}
