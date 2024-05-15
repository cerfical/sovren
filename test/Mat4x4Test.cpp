#include <gtest/gtest.h>

#include <reni/math.hpp>
#include <reni/print.hpp>

class Mat4x4 : public testing::Test {
protected:
	const reni::Mat4x4 m1 = {
		1.0, 2.0, 3.0, 4.0,
		5.0, 6.0, 7.0, 8.0,
		9.0, 10.0, 11.0, 12.0,
		13.0, 14.0, 15.0, 16.0
	};

	const reni::Mat4x4 m2 = {
		2.0, 4.0, 6.0, 8.0,
		10.0, 12.0, 14.0, 16.0,
		18.0, 20.0, 22.0, 24.0,
		26.0, 28.0, 30.0, 32.0,
	};

	const reni::Mat4x4 m3 = {
		180.0, 200.0, 220.0, 240.0,
		404.0, 456.0, 508.0, 560.0,
		628.0, 712.0, 796.0, 880.0,
		852.0, 968.0, 1084.0, 1200.0
	};

	reni::Mat4x4 m;
};

TEST_F(Mat4x4, IterationOverComponents) {
	float nextVal = 0.0;
	for(const auto& row : m1) {
		for(const auto& col : row) {
			EXPECT_EQ(col, (nextVal += 1.0));
		}
	}
	EXPECT_EQ(nextVal, 16.0);
}

TEST_F(Mat4x4, MutatingIterationOverComponents) {
	float nextVal = 0.0;

	for(auto& row : m) {
		for(auto& col : row) {
			col = (nextVal += 1.0);
		}
	}
	
	EXPECT_EQ(nextVal, 16.0);
	EXPECT_EQ(m, m1);
}

TEST_F(Mat4x4, EqualityComparison) {
	EXPECT_EQ(m1, m1);
	EXPECT_NE(m1, m2);
}

TEST_F(Mat4x4, Addition) {
	ASSERT_EQ(m1 + m1, m2);
}

TEST_F(Mat4x4, Subtraction) {
	ASSERT_EQ(m2 - m1, m1);
}

TEST_F(Mat4x4, Multiplication) {
	ASSERT_EQ(m1 * m2, m3);
}
