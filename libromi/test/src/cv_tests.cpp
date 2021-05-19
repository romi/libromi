#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/cv.h"

using namespace std;
using namespace testing;
using namespace romi;


class cv_tests : public ::testing::Test
{
protected:
        
	cv_tests() {
        }

	~cv_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}
};

TEST_F(cv_tests, test_segment_crosses_white_area)
{
        // Arrange
        uint8_t data[] = {
                0, 0, 255, 0, 0,
                0, 0, 255, 0, 0,
                0, 0, 255, 0, 0,
                0, 0, 255, 0, 0,
                0, 0, 255, 0, 0
        };
        Image image(Image::BW, data, 5, 5);

        // Act
        bool test1 = segment_crosses_white_area(image, v3(0, 0, 0), v3(4, 0, 0));
        bool test2 = segment_crosses_white_area(image, v3(0, 0, 0), v3(0, 4, 0));

        // Assert
        ASSERT_TRUE(test1);
        ASSERT_FALSE(test2);
}

TEST_F(cv_tests, test_sort_centers_1)
{
        // Arrange
        uint8_t data[] = {
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
                0, 0, 0, 0,
        };
        Image components(Image::BW, data, 4, 4);
        Centers centers;
        centers.emplace_back(0, 0);
        centers.emplace_back(2, 0);
        centers.emplace_back(0, 2);
        centers.emplace_back(2, 2);

        // Act
        std::vector<Centers> list = sort_centers(centers, components);

        // Assert
        ASSERT_EQ(list.size(), 1);
        ASSERT_EQ(list[0].size(), 4);
}

TEST_F(cv_tests, test_sort_centers_2)
{
        // Arrange
        uint8_t data[] = {
                0, 0, 1, 1,
                0, 0, 1, 1,
                2, 2, 3, 3,
                2, 2, 3, 3,
        };
        Image components(Image::BW, data, 4, 4);
        Centers centers;
        centers.emplace_back(0, 0);
        centers.emplace_back(2, 0);
        centers.emplace_back(0, 2);
        centers.emplace_back(2, 2);

        // Act
        std::vector<Centers> list = sort_centers(centers, components);

        // Assert
        ASSERT_EQ(list.size(), 4);
        ASSERT_EQ(list[0].size(), 1);
        ASSERT_EQ(list[1].size(), 1);
        ASSERT_EQ(list[2].size(), 1);
        ASSERT_EQ(list[3].size(), 1);
}
