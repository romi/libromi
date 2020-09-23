#include <string>
#include "gtest/gtest.h"

#include <r.h>

extern "C" {
#include "fff.h"
#include "cnc_range.h"
}

class cnc_range_tests : public ::testing::Test
{
protected:
	cnc_range_tests() = default;

	~cnc_range_tests() override = default;

	void SetUp() override
    {
	}

	void TearDown() override
    {
	}

};

TEST_F(cnc_range_tests, test_cnc_range_parse_success)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[2,3],[4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_EQ(err, 0);
        ASSERT_EQ(range.x[0], 0);
        ASSERT_EQ(range.x[1], 1);
        ASSERT_EQ(range.y[0], 2);
        ASSERT_EQ(range.y[1], 3);
        ASSERT_EQ(range.z[0], 4);
        ASSERT_EQ(range.z[1], 5);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_1)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1,2],[3,4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_2)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[2,3],[4,5,6]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_3)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[0,1,2,3,4,5,6]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_4)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[0,1],[2,\"3\"],[4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_5)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_fails_on_bad_json_6)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("{'x':[0,1],'y':[2,3],'z':[4,5]}");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_is_valid)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[2,3],[4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        //Assert
        ASSERT_EQ(err, 0);
        ASSERT_EQ(1, cnc_range_is_valid(&range, 0, 2, 4));
        ASSERT_EQ(1, cnc_range_is_valid(&range, 1, 3, 5));
        ASSERT_EQ(1, cnc_range_is_valid(&range, 0.5, 2.5, 4.5));
        ASSERT_EQ(0, cnc_range_is_valid(&range, -1, 2, 4));
        ASSERT_EQ(0, cnc_range_is_valid(&range, 0, -2, 4));
        ASSERT_EQ(0, cnc_range_is_valid(&range, 0, 2, -4));
        ASSERT_EQ(0, cnc_range_is_valid(&range, 2, 2, 4));
        ASSERT_EQ(0, cnc_range_is_valid(&range, 0, 4, 4));
        ASSERT_EQ(0, cnc_range_is_valid(&range, 0, 2, 6));
}

TEST_F(cnc_range_tests, test_cnc_range_parse_set_minmax)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[2,3],[4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);

        cnc_range_set_minmax(&range, 0, -100, 100);
        cnc_range_set_minmax(&range, 1, -200, 200);
        cnc_range_set_minmax(&range, 2, -300, 300);

        json_unref(r);
        
        //Assert
        ASSERT_EQ(err, 0);
        ASSERT_EQ(range.x[0], -100);
        ASSERT_EQ(range.x[1],  100); 
        ASSERT_EQ(range.y[0], -200);
        ASSERT_EQ(range.y[1],  200);
        ASSERT_EQ(range.z[0], -300);
        ASSERT_EQ(range.z[1],  300);
}

TEST_F(cnc_range_tests, test_cnc_range_parse_set_max_lt_min)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[2,3],[4,5]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        err = cnc_range_set_minmax(&range, 0, 100, -100);
        json_unref(r);
        
        //Assert
        ASSERT_NE(err, 0);

}

TEST_F(cnc_range_tests, test_cnc_range_outofbounds_error)
{
        // Arrange
        cnc_range_t range;

        json_object_t r = json_parse("[[0,1],[0,1],[0,1]]");
        
        // Act
        int err = cnc_range_parse(&range, r);
        json_unref(r);
        
        double d1 = cnc_range_error(&range, 0, 0, 0);
        double d2 = cnc_range_error(&range, 1, 0, 0);
        double d3 = cnc_range_error(&range, 0, 1, 0);
        double d4 = cnc_range_error(&range, 1, 1, 0);
        double d5 = cnc_range_error(&range, 0, 0, 1);
        double d6 = cnc_range_error(&range, 1, 0, 1);
        double d7 = cnc_range_error(&range, 0, 1, 1);
        double d8 = cnc_range_error(&range, 1, 1, 1);

        double d9 = cnc_range_error(&range, 2, 0, 0);
        double d10 = cnc_range_error(&range, 0, -1, 0);
        double d11 = cnc_range_error(&range, 0, 2, 0);
        double d12 = cnc_range_error(&range, 0, -1, 0);

        double d13 = cnc_range_error(&range, 2, 2, 0);
        double d14 = cnc_range_error(&range, 2, 2, 2);
        
        //Assert
        ASSERT_EQ(err, 0);
        ASSERT_EQ(d1, 0);
        ASSERT_EQ(d2, 0);
        ASSERT_EQ(d3, 0);
        ASSERT_EQ(d4, 0);
        ASSERT_EQ(d5, 0);
        ASSERT_EQ(d6, 0);
        ASSERT_EQ(d7, 0);
        ASSERT_EQ(d8, 0);
        
        ASSERT_GT(d9, 0.99);
        ASSERT_LT(d9, 1.01);
        ASSERT_GT(d10, 0.99);
        ASSERT_LT(d10, 1.01);
        ASSERT_GT(d11, 0.99);
        ASSERT_LT(d11, 1.01);
        ASSERT_GT(d12, 0.99);
        ASSERT_LT(d12, 1.01);

        ASSERT_GT(d13, 1.4);
        ASSERT_LT(d13, 1.5);
        ASSERT_GT(d14, 1.7);
        ASSERT_LT(d14, 1.8);
}
