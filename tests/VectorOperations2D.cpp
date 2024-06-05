#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

class VectorOperations2D : public testing::Test {
protected:
	const Vec2 v1 = { 1, 2 };
	const Vec2 v2 = { 2, 4 };
};

TEST_F(VectorOperations2D, IterationOverComponents) {
	float nextVal = 0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1));
	}
	EXPECT_EQ(nextVal, 2);
}

TEST_F(VectorOperations2D, MutatingIterationOverComponents) {
	float nextVal = 0;
	Vec2 v;

	for(auto& val : v) {
		val = (nextVal += 1);
	}
	
	EXPECT_EQ(nextVal, 2);
	EXPECT_EQ(v, v1);
}

TEST_F(VectorOperations2D, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(VectorOperations2D, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(VectorOperations2D, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(VectorOperations2D, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(VectorOperations2D, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(VectorOperations2D, ComponentWiseMultiplication) {
	const auto v3 = Vec2(2, 8);
	ASSERT_EQ(v1 * v2, v3);
}

TEST_F(VectorOperations2D, ComponentWiseDivision) {
	const auto v3 = Vec2(2, 2);
	ASSERT_EQ(v2 / v1, v3);
}

TEST_F(VectorOperations2D, ScalarAddition) {
	const auto v3 = Vec2(6, 7);
	ASSERT_EQ(v1 + 5, v3);
}

TEST_F(VectorOperations2D, ScalarSubtraction) {
	const auto v3 = Vec2(0, 2);
	ASSERT_EQ(v2 - 2, v3);
}

TEST_F(VectorOperations2D, ScalarMultiplication) {
	ASSERT_EQ(v1 * 2, v2);
}

TEST_F(VectorOperations2D, ScalarDivision) {
	ASSERT_EQ(v2 / 2, v1);
}

TEST_F(VectorOperations2D, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 10);
}

TEST_F(VectorOperations2D, MatrixMultiplication) {
	const auto m = Mat2x2(
		2, 0,
		0, 2
	);
	ASSERT_EQ(v1 * m, v2);
}
