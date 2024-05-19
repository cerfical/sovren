#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

class MatrixOperations4D : public testing::Test {
protected:
	const Mat4x4 m1 = {
		1,  2,  3,  4,
		5,  6,  7,  8,
		9,  10, 11, 12,
		13, 14, 15, 16
	};

	const Mat4x4 m2 = {
		2,  4,  6,  8,
		10, 12, 14, 16,
		18, 20, 22, 24,
		26, 28, 30, 32,
	};
};

TEST_F(MatrixOperations4D, IterationOverComponents) {
	float nextVal = 0;
	for(const auto& row : m1) {
		for(const auto& col : row) {
			EXPECT_EQ(col, (nextVal += 1));
		}
	}
	EXPECT_EQ(nextVal, 16);
}

TEST_F(MatrixOperations4D, MutatingIterationOverComponents) {
	float nextVal = 0;
	Mat4x4 m;

	for(auto& row : m) {
		for(auto& col : row) {
			col = (nextVal += 1);
		}
	}
	
	EXPECT_EQ(nextVal, 16);
	EXPECT_EQ(m, m1);
}

TEST_F(MatrixOperations4D, EqualityComparison) {
	EXPECT_EQ(m1, m1);
	EXPECT_NE(m1, m2);
}

TEST_F(MatrixOperations4D, Addition) {
	ASSERT_EQ(m1 + m1, m2);
}

TEST_F(MatrixOperations4D, Subtraction) {
	ASSERT_EQ(m2 - m1, m1);
}

TEST_F(MatrixOperations4D, Multiplication) {
	const auto m3 = Mat4x4(
		180, 200, 220,  240,
		404, 456, 508,  560,
		628, 712, 796,  880,
		852, 968, 1084, 1200
	);
	ASSERT_EQ(m1 * m2, m3);
}

TEST_F(MatrixOperations4D, ScalarAddition) {
	const auto m3 = Mat4x4(
		6,  7,  8,  9,
		10, 11, 12, 13,
		14, 15, 16, 17,
		18, 19, 20, 21
	);
	ASSERT_EQ(m1 + 5, m3);
}

TEST_F(MatrixOperations4D, ScalarSubtraction) {
	const Mat4x4 m3 = {
		0,  2,  4,  6,
		8,  10, 12, 14,
		16, 18, 20, 22,
		24, 26, 28, 30
	};
	ASSERT_EQ(m2 - 2, m3);
}

TEST_F(MatrixOperations4D, ScalarMultiplication) {
	ASSERT_EQ(m1 * 2, m2);
}

TEST_F(MatrixOperations4D, ScalarDivision) {
	ASSERT_EQ(m2 / 2, m1);
}

