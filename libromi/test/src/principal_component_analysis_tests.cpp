#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "cv/PrincipalComponentAnalysis.h"

using namespace std;
using namespace testing;
using namespace romi;


class principal_component_analysis_tests : public ::testing::Test
{
protected:
        
	principal_component_analysis_tests() = default;

	~principal_component_analysis_tests() override = default;

	void SetUp() override {}

	void TearDown() override {}
};

TEST_F(principal_component_analysis_tests, can_construct)
{
    // Arrange
    // Act
    // Assert
    ASSERT_NO_THROW(romi::PrincipalComponentAnalysis pca);

}

TEST_F(principal_component_analysis_tests, empty_image_throws_runtime_error)
{
    // Arrange
    int width = 10;
    int height = 10;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    std::vector<uint8_t> image_data;
    romi::PrincipalComponentAnalysis pca;

    // Act
    // Assert
    ASSERT_THROW(pca.PCA(image_data, channels, width, height, orientation, cte), std::runtime_error);
}

TEST_F(principal_component_analysis_tests, invalid_channel_number_throws_runtime_error)
{
    // Arrange
    int width = 10;
    int height = 10;
    int channels = 3;
    double cte = 0;
    double orientation = 0;
    std::vector<uint8_t> image_data{0, 0, 0, 0, 0};
    romi::PrincipalComponentAnalysis pca;

    // Act
    // Assert
    ASSERT_THROW(pca.PCA(image_data, channels, width, height, orientation, cte), std::runtime_error);
}

TEST_F(principal_component_analysis_tests, width_0_throws_runtime_error)
{
    // Arrange
    int width = 0;
    int height = 10;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    std::vector<uint8_t> image_data{0, 0, 0, 0, 0};
    romi::PrincipalComponentAnalysis pca;

    // Act
    // Assert
    ASSERT_THROW(pca.PCA(image_data, channels, width, height, orientation, cte), std::runtime_error);
}

TEST_F(principal_component_analysis_tests, height_0_throws_runtime_error)
{
    // Arrange
    int width = 10;
    int height = 0;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    std::vector<uint8_t> image_data{0, 0, 0, 0, 0};
    romi::PrincipalComponentAnalysis pca;

    // Act
    // Assert
    ASSERT_THROW(pca.PCA(image_data, channels, width, height, orientation, cte), std::runtime_error);
}

TEST_F(principal_component_analysis_tests, orientation_correct_at_minus_45_degrees)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = -45;

    std::vector<uint8_t> image_data{    255, 000, 000, 000, 000,
                                        000, 255, 000, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 000, 255, 000,
                                        000, 000, 000, 000, 255};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(0, cte);
    ASSERT_EQ(expected_orientation_radians, orientation);
}

TEST_F(principal_component_analysis_tests, orientation_correct_at_45_degrees)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 45;

    std::vector<uint8_t> image_data{    000, 000, 000, 000, 255,
                                        000, 000, 000, 255, 000,
                                        000, 000, 255, 000, 000,
                                        000, 255, 000, 000, 000,
                                        255, 000, 000, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(0, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, orientation_correct_at_0_degrees)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;

    std::vector<uint8_t> image_data{    000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(0, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, orientation_correct_at_90_degrees)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 90;

    std::vector<uint8_t> image_data{    000, 000, 000, 000, 000,
                                        000, 000, 000, 000, 000,
                                        000, 255, 255, 255, 255,
                                        000, 000, 000, 000, 000,
                                        000, 000, 000, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(0, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, cte_correct_at_center)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;

    std::vector<uint8_t> image_data{    000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000,
                                        000, 000, 255, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(0, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, cte_correct_1_left)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;
    double expected_cte = 1;

    std::vector<uint8_t> image_data{    000, 255, 000, 000, 000,
                                        000, 255, 000, 000, 000,
                                        000, 255, 000, 000, 000,
                                        000, 255, 000, 000, 000,
                                        000, 255, 000, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(expected_cte, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, cte_correct_2_left)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;
    double expected_cte = 2;

    std::vector<uint8_t> image_data{    255, 000, 000, 000,000,
                                        255, 000, 000, 000, 000,
                                        255, 000, 000, 000, 000,
                                        255, 000, 000, 000, 000,
                                        255, 000, 000, 000, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(expected_cte, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, cte_correct_1_right)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;
    double expected_cte = -1;

    std::vector<uint8_t> image_data{    000, 000, 000, 255, 000,
                                        000, 000, 000, 255, 000,
                                        000, 000, 000, 255, 000,
                                        000, 000, 000, 255, 000,
                                        000, 000, 000, 255, 000};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(expected_cte, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}

TEST_F(principal_component_analysis_tests, cte_correct_2_right)
{
    // Arrange
    int width = 5;
    int height = 5;
    int channels = 1;
    double cte = 0;
    double orientation = 0;
    double degrees = 0;
    double expected_cte = -2;

    std::vector<uint8_t> image_data{    000, 000, 000, 000, 255,
                                        000, 000, 000, 000, 255,
                                        000, 000, 000, 000, 255,
                                        000, 000, 000, 000, 255,
                                        000, 000, 000, 000, 255};

    double expected_orientation_radians = degrees * (M_PI/180);
    romi::PrincipalComponentAnalysis pca;

    // Act
    pca.PCA(image_data, channels, width, height, orientation, cte);

    // Assert
    ASSERT_EQ(expected_cte, cte);
    ASSERT_NEAR(expected_orientation_radians, orientation, 0.000001);
}