#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

class VectorOperations4D : public testing::Test {
protected:
	const Vec4 v1 = { 1, 2, 3, 4 };
	const Vec4 v2 = { 2, 4, 6, 8 };
};

TEST_F(VectorOperations4D, IterationOverComponents) {
	float nextVal = 0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1));
	}
	EXPECT_EQ(nextVal, 4);
}

TEST_F(VectorOperations4D, MutatingIterationOverComponents) {
	float nextVal = 0;
	Vec4 v;

	for(auto& val : v) {
		val = (nextVal += 1);
	}
	
	EXPECT_EQ(nextVal, 4);
	EXPECT_EQ(v, v1);
}

TEST_F(VectorOperations4D, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(VectorOperations4D, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(VectorOperations4D, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(VectorOperations4D, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(VectorOperations4D, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(VectorOperations4D, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 60);
}

TEST_F(VectorOperations4D, MatrixMultiplication) {
	const auto m = Mat4x4(
		2, 0, 0, 0,
		0, 2, 0, 0,
		0, 0, 2, 0,
		0, 0, 0, 2
	);
	ASSERT_EQ(v1 * m, v2);
}
