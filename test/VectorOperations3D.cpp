#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

class VectorOperations3D : public testing::Test {
protected:
	const Vec3 v1 = { 1, 2, 3 };
	const Vec3 v2 = { 2, 4, 6 };
};

TEST_F(VectorOperations3D, IterationOverComponents) {
	float nextVal = 0;
	for(const auto& val : v1) {
		EXPECT_EQ(val, (nextVal += 1));
	}
	EXPECT_EQ(nextVal, 3);
}

TEST_F(VectorOperations3D, MutatingIterationOverComponents) {
	float nextVal = 0;
	Vec3 v;
	
	for(auto& val : v) {
		val = (nextVal += 1);
	}
	
	EXPECT_EQ(nextVal, 3);
	EXPECT_EQ(v, v1);
}

TEST_F(VectorOperations3D, EqualityComparison) {
	EXPECT_EQ(v1, v1);
	EXPECT_NE(v1, v2);
}

TEST_F(VectorOperations3D, GreaterThanComparison) {
	EXPECT_GT(v2, v1);
	EXPECT_LE(v1, v2);
}

TEST_F(VectorOperations3D, LessThanComparison) {
	EXPECT_LT(v1, v2);
	EXPECT_GE(v2, v1);
}

TEST_F(VectorOperations3D, Addition) {
	ASSERT_EQ(v1 + v1, v2);
}

TEST_F(VectorOperations3D, Subtraction) {
	ASSERT_EQ(v2 - v1, v1);
}

TEST_F(VectorOperations3D, DotProduct) {
	ASSERT_EQ(dot(v1, v2), 28);
}

TEST_F(VectorOperations3D, MatrixMultiplication) {
	const auto m = Mat3x3(
		2, 0, 0,
		0, 2, 0,
		0, 0, 2
	);
	ASSERT_EQ(v1 * m, v2);
}
