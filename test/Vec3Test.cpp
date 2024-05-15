#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Vec3 : public testing::Test {
protected:
	const reni::Vec3 v1 = { 1.0, 2.0, 3.0 };
	const reni::Vec3 v2 = { 2.0, 4.0, 6.0 };
	
	const reni::Mat3x3 m1 = {
		2.0, 0.0, 0.0,
		0.0, 2.0, 0.0,
		0.0, 0.0, 2.0
	};

	reni::Vec3 v;
};

TEST_F(Vec3, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 3.0);
}

TEST_F(Vec3, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 3.0);
	EXPECT_EQ(v, v1);
}

TEST_F(Vec3, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(Vec3, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(Vec3, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(Vec3, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(Vec3, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(Vec3, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 28.0);
}

TEST_F(Vec3, MatrixMultiplication) {
	ASSERT_EQ(v1 * m1, v2);
}
