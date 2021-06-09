#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <r.h>
#include "rover/L1NavigationController.h"

using namespace std;
using namespace testing;
using namespace romi;

class l1_navigationcontroller_tests : public ::testing::Test
{
protected:
        
	l1_navigationcontroller_tests() {
	}

	~l1_navigationcontroller_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}
};

TEST_F(l1_navigationcontroller_tests, constructor_with_good_values_succeeds)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(1.0, 2.0);

                // Assert
                
        } catch (const std::exception& e) {
                // Assert
                FAIL() << "Didn't expect an exception";
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_width_throws_error_1)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(0.0, 2.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_width_throws_error_2)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(-1.0, 2.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_width_throws_error_3)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(10.0, 2.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_1)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(1.0, 0.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_2)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(1.0, 0.5);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, constructor_with_bad_distance_throws_error_3)
{
        // Arrange

        try {
                // Act
                L1NavigationController ctrl(1.0, 100.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_on_straight_line)
{
        // Arrange
        L1NavigationController ctrl(1.0, 1.0);

        // Act
        double correction = ctrl.estimate_correction(0.0, 0.0);

        // Assert
        ASSERT_EQ(correction, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_throws_exception_on_error_too_big)
{
                // Arrange
        L1NavigationController ctrl(1.0, 1.0);
        
        try {
                // Act
                ctrl.estimate_correction(1.01, 0.0);

                // Assert
                FAIL() << "Expected an exception";
                
        } catch (const std::exception& e) {
                // Assert
        }        
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_returns_negative_correction_on_negative_offset)
{
        // Arrange
        L1NavigationController ctrl(1.0, 1.0);

        // Act
        double correction = ctrl.estimate_correction(-0.1, 0.0);

        // Assert
        ASSERT_LT(correction, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_returns_positive_correction_on_positive_offset)
{
        // Arrange
        L1NavigationController ctrl(1.0, 1.0);

        // Act
        double correction = ctrl.estimate_correction(0.1, 0.0);

        // Assert
        ASSERT_GT(correction, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_returns_negative_correction_on_negative_orientation)
{
        // Arrange
        L1NavigationController ctrl(1.0, 1.0);

        // Act
        double correction = ctrl.estimate_correction(0.0, -0.1);

        // Assert
        ASSERT_LT(correction, 0.0);
}

TEST_F(l1_navigationcontroller_tests, estimate_correction_returns_positive_correction_on_positive_orientation)
{
        // Arrange
        L1NavigationController ctrl(1.0, 1.0);

        // Act
        double correction = ctrl.estimate_correction(0.1, 0.1);

        // Assert
        ASSERT_GT(correction, 0.0);
}


TEST_F(l1_navigationcontroller_tests, estimate_correction_returns_expected_value)
{
        // Arrange
        double w = 1.0;
        double L = 2.0;
        double d = -0.2;
        double R = (d * d + L * L) / (2.0 * d);
        double expected = w / (2.0 * R);
        L1NavigationController ctrl(w, L);
                
        // Act
        double correction = ctrl.estimate_correction(d, 0.0);

        r_debug("correction %f, expected %f", correction, expected);
        
        // Assert
        ASSERT_NEAR(correction, expected, 0.01);
}
