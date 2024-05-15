#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Mat3x3 : public testing::Test {
protected:
	const reni::Mat3x3 m1 = {
		1.0, 2.0, 3.0,
		4.0, 5.0, 6.0,
		7.0, 8.0, 9.0
	};

	const reni::Mat3x3 m2 = {
		2.0, 4.0, 6.0,
		8.0, 10.0, 12.0,
		14.0, 16.0, 18.0
	};

	reni::Mat3x3 m;
};

TEST_F(Mat3x3, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& row : m1) {
		for(const auto& col : row) {
			EXPECT_EQ(col, (nextVal += 1.0));
		}
	}
	EXPECT_EQ(nextVal, 9.0);
}

TEST_F(Mat3x3, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& row : m) {
		for(auto& col : row) {
			col = (nextVal += 1.0);
		}
	}
	
	EXPECT_EQ(nextVal, 9.0);
	EXPECT_EQ(m, m1);
}

TEST_F(Mat3x3, EqualityComparison) {
	EXPECT_EQ(m1, m1);
	EXPECT_NE(m1, m2);
}

TEST_F(Mat3x3, Addition) {
	ASSERT_EQ(m1 + m1, m2);
}

TEST_F(Mat3x3, Subtraction) {
	ASSERT_EQ(m2 - m1, m1);
}
