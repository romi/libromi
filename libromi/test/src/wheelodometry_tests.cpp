#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "rover/WheelOdometry.h"
#include "../mock/mock_motordriver.h"

using namespace std;
using namespace testing;
using namespace romi;

class wheelodometry_tests : public ::testing::Test
{
protected:
        JsonCpp config;
        double epsilon;
        MockMotorDriver driver_;
        double left_;
        double right_;
        double timestamp_;
        
	wheelodometry_tests()
                : config(),
                  epsilon(0.000001),
                  driver_(),
                  left_(0.0),
                  right_(0.0),
                  timestamp_(0.0) {
                const char * config_string = "{"
                        "'wheel_diameter': 1.0,"
                        "'wheel_base': 1.0,"
                        "'encoder_steps': 1000.0,"
                        "'maximum_speed': 3.0 }";
                config = JsonCpp::parse(config_string);
	}

	~wheelodometry_tests() override = default;

	void SetUp() override {
	}

	void TearDown() override {
	}

	void init_encoders(double left, double right, double timestamp) {
                left_ = left;
                right_ = right;
                timestamp_ = timestamp;

                EXPECT_CALL(driver_, get_encoder_values)
                        .WillOnce(DoAll(SetArgReferee<0>(left_),
                                        SetArgReferee<1>(right_),
                                        SetArgReferee<2>(timestamp_),
                                        Return(true)));
	}
};

TEST_F(wheelodometry_tests, test_initialisation)
{
        // Arrange
        NavigationSettings rover(config);
        init_encoders(1000.0, 1000.0, 0.0);
        
        // Act
        WheelOdometry odometry(rover, driver_);
        
        // Assert
        v3 pos = odometry.get_location();
        double orientation = odometry.get_orientation();
                
        ASSERT_NEAR(pos.x(), 0.0, epsilon);
        ASSERT_NEAR(pos.y(), 0.0, epsilon);
        ASSERT_NEAR(orientation, 0.0, epsilon);
}

TEST_F(wheelodometry_tests, test_update_encoders_displacement)
{
        // Arrange
        NavigationSettings rover(config);
        init_encoders(1000.0, 1000.0, 0.0);
        WheelOdometry odometry(rover, driver_);

        // Act
        // 1000 steps = 1 revolution = 3.14 meters
        odometry.set_encoders(2000.0, 2000.0, 1.0);
        
        // Assert
        v3 pos = odometry.get_location();
        v3 speed = odometry.get_speed();
        double orientation = odometry.get_orientation();
                
        ASSERT_NEAR(pos.x(), M_PI, epsilon);
        ASSERT_NEAR(pos.y(), 0.0, epsilon);
        ASSERT_NEAR(speed.x(), 0.2 * M_PI, epsilon);
        ASSERT_NEAR(speed.y(), 0.0, epsilon);
        ASSERT_NEAR(orientation, 0.0, epsilon);
}

TEST_F(wheelodometry_tests, test_update_encoders_orientation_360degrees)
{
        // Arrange
        NavigationSettings rover(config);
        init_encoders(0.0, 0.0, 0.0);
        WheelOdometry odometry(rover, driver_);

        // Act
        // left: 1000 steps = 1 revolution = 3.14 meters
        // right: 1000 steps = 1 revolution = -3.14 meters
        // -> full rotation in the same spot (wheel base = 1m)
        odometry.set_encoders(1000.0, -1000.0, 1.0);
        
        // Assert
        v3 pos = odometry.get_location();
        double orientation = odometry.get_orientation();
                
        ASSERT_NEAR(pos.x(), 0, epsilon);
        ASSERT_NEAR(pos.y(), 0.0, epsilon);
        ASSERT_NEAR(orientation, -2.0 * M_PI, epsilon);
}

TEST_F(wheelodometry_tests, test_update_encoders_orientation_90degrees)
{
        // Arrange
        NavigationSettings rover(config);
        init_encoders(0.0, 0.0, 0.0);
        WheelOdometry odometry(rover, driver_);

        // Act
        // 90° rotation
        odometry.set_encoders(-250.0, 250.0, 1.0);
        
        //Assert
        v3 pos = odometry.get_location();
        double orientation = odometry.get_orientation();
                
        ASSERT_NEAR(pos.x(), 0, epsilon);
        ASSERT_NEAR(pos.y(), 0.0, epsilon);
        ASSERT_NEAR(orientation, M_PI / 2.0, epsilon);
}

TEST_F(wheelodometry_tests, test_update_encoders_move_and_turn_90degrees)
{
        NavigationSettings rover(config);
        init_encoders(0.0, 0.0, 0.0);
        WheelOdometry odometry(rover, driver_);

        odometry.set_encoders(1000.0, 1000.0, 1.0);
        odometry.set_encoders(1000.0 - 250.0, 1000.0 + 250.0, 1.0);
        
        //Assert
        v3 pos = odometry.get_location();
        double orientation = odometry.get_orientation();
                
        ASSERT_NEAR(pos.x(), M_PI, epsilon);
        ASSERT_NEAR(pos.y(), 0.0, epsilon);
        ASSERT_NEAR(orientation, M_PI / 2.0, epsilon);
}

TEST_F(wheelodometry_tests, test_update_encoders_2x90degrees)
{
        // Arrange
        NavigationSettings rover(config);
        init_encoders(0.0, 0.0, 0.0);
        WheelOdometry odometry(rover, driver_);
        double wheel_base = 1.0;
        double wheel_circumference = M_PI;

        // Act
        // 90° rotation around the left wheel
        odometry.set_encoders(0.0,
                              M_PI * 1000.0 / 2.0 / wheel_circumference,
                              1.0);
        
        // Assert
        v3 pos = odometry.get_location();
        double orientation = odometry.get_orientation();
        
        ASSERT_NEAR(pos.x(), wheel_base/2.0, epsilon);
        ASSERT_NEAR(pos.y(), wheel_base/2.0, epsilon);
        ASSERT_NEAR(orientation, M_PI/2.0, epsilon);

        // Act
        // 90° rotation around the right wheel
        odometry.set_encoders(M_PI * 1000.0 / 2.0 / wheel_circumference,
                              M_PI * 1000.0 / 2.0 / wheel_circumference,
                              2.0);
        
        // Assert
        pos = odometry.get_location();
        orientation = odometry.get_orientation();
        
        ASSERT_NEAR(pos.x(), wheel_base, epsilon);
        ASSERT_NEAR(pos.y(), wheel_base, epsilon);
        ASSERT_NEAR(orientation, 0.0, epsilon);
}
