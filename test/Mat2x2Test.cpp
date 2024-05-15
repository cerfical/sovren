#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Mat2x2 : public testing::Test {
protected:
	const reni::Mat2x2 m1 = {
		1.0, 2.0,
		3.0, 4.0
	};

	const reni::Mat2x2 m2 = {
		2.0, 4.0,
		6.0, 8.0
	};

	const reni::Mat2x2 m3 = {
		14.0, 20.0,
		30.0, 44.0
	};

	reni::Mat2x2 m;
};

TEST_F(Mat2x2, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& row : m1) {
		for(const auto& col : row) {
			EXPECT_EQ(col, (nextVal += 1.0));
		}
	}
	EXPECT_EQ(nextVal, 4.0);
}

TEST_F(Mat2x2, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& row : m) {
		for(auto& col : row) {
			col = (nextVal += 1.0);
		}
	}
	
	EXPECT_EQ(nextVal, 4.0);
	EXPECT_EQ(m, m1);
}

TEST_F(Mat2x2, EqualityComparison) {
	EXPECT_EQ(m1, m1);
	EXPECT_NE(m1, m2);
}

TEST_F(Mat2x2, Addition) {
	ASSERT_EQ(m1 + m1, m2);
}

TEST_F(Mat2x2, Subtraction) {
	ASSERT_EQ(m2 - m1, m1);
}

TEST_F(Mat2x2, Multiplication) {
	ASSERT_EQ(m1 * m2, m3);
}
