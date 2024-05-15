#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Vec2 : public testing::Test {
protected:
	const reni::Vec2 v1 = { 1.0, 2.0 };
	const reni::Vec2 v2 = { 2.0, 4.0 };
	
	const reni::Mat2x2 m1 = {
		2.0, 0.0,
		0.0, 2.0
	};

	reni::Vec2 v;
};

TEST_F(Vec2, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1.0));
	}
	EXPECT_EQ(nextVal, 2.0);
}

TEST_F(Vec2, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& val : v) {
		val = (nextVal += 1.0);
	}
	
	EXPECT_EQ(nextVal, 2.0);
	EXPECT_EQ(v, v1);
}

TEST_F(Vec2, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(Vec2, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(Vec2, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(Vec2, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(Vec2, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(Vec2, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 10.0);
}

TEST_F(Vec2, MatrixMultiplication) {
	ASSERT_EQ(v1 * m1, v2);
}
