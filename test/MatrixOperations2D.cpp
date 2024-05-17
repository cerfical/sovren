#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

using namespace reni;

class MatrixOperations2D : public testing::Test {
protected:
	const Mat2x2 m1 = {
		1, 2,
		3, 4
	};

	const Mat2x2 m2 = {
		2, 4,
		6, 8
	};
};

TEST_F(MatrixOperations2D, IterationOverComponents) {
	float nextVal = 0;
	for(const auto& row : m1) {
		for(const auto& col : row) {
			EXPECT_EQ(col, (nextVal += 1));
		}
	}
	EXPECT_EQ(nextVal, 4);
}

TEST_F(MatrixOperations2D, MutatingIterationOverComponents) {
	float nextVal = 0;
	Mat2x2 m;

	for(auto& row : m) {
		for(auto& col : row) {
			col = (nextVal += 1);
		}
	}
	
	EXPECT_EQ(nextVal, 4);
	EXPECT_EQ(m, m1);
}

TEST_F(MatrixOperations2D, EqualityComparison) {
	EXPECT_EQ(m1, m1);
	EXPECT_NE(m1, m2);
}

TEST_F(MatrixOperations2D, Addition) {
	ASSERT_EQ(m1 + m1, m2);
}

TEST_F(MatrixOperations2D, Subtraction) {
	ASSERT_EQ(m2 - m1, m1);
}

TEST_F(MatrixOperations2D, Multiplication) {
	const auto m3 = Mat2x2(
		14, 20,
		30, 44
	);
	ASSERT_EQ(m1 * m2, m3);
}
