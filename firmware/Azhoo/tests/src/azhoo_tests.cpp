#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Azhoo.h"
#include "../mocks/mock_arduino.h"
#include "../mocks/mock_encoder.h"
#include "../mocks/mock_pwm.h"

using namespace std;
using namespace testing;

class azhoo_tests : public ::testing::Test
{
protected:
        MockArduino arduino_;
        MockEncoder left_encoder_;
        MockEncoder right_encoder_;
        MockPWM left_pwm_;
        MockPWM right_pwm_;
        AzhooConfiguration config_;
        
	azhoo_tests()
                : arduino_(),
                  left_encoder_(),
                  right_encoder_(),
                  left_pwm_(),
                  right_pwm_(),
                  config_() {
                make_default_configuration(config_);
        }

	~azhoo_tests() override = default;

	void SetUp() override {
        }

	void TearDown() override {}

	void make_default_configuration(AzhooConfiguration& config) {
                config.encoder_steps = 10000;
                config.left_direction = 1;
                config.right_direction = -1;
                config.max_speed = 1.0;
                config.max_acceleration = 1.0;
                config.kp_numerator = 1;
                config.kp_denominator = 1;
                config.ki_numerator = 1;
                config.ki_denominator = 1;
        }
};

TEST_F(azhoo_tests, test_constructor)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        
        // Act
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);

        // Assert
        ASSERT_EQ(azhoo.interval_, Azhoo::kDefaultUpdateInterval);
        ASSERT_EQ(azhoo.last_time_, 0);
}

TEST_F(azhoo_tests, test_setup)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        
        // Act
        azhoo.setup();
        
        // Assert
        ASSERT_EQ(azhoo.last_time_, 1000);
        ASSERT_TRUE(azhoo.is_set_up());
        ASSERT_FALSE(azhoo.is_configured());
}

TEST_F(azhoo_tests, test_configuration)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        EXPECT_CALL(left_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(right_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(arduino_, init_encoders(10000, 1, -1));
        EXPECT_CALL(left_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(left_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(right_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(right_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        
        // Act
        azhoo.configure(config_);
        
        // Assert
        ASSERT_FALSE(azhoo.is_set_up());
        ASSERT_TRUE(azhoo.is_configured());
}

TEST_F(azhoo_tests, test_enable_fails_if_not_configured)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        
        // Act
        bool success = azhoo.enable();
        
        // Assert
        ASSERT_FALSE(success);
}

TEST_F(azhoo_tests, test_update_fails_if_not_configured)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        
        // Act
        bool success = azhoo.update();
        
        // Assert
        ASSERT_FALSE(success);
}

TEST_F(azhoo_tests, test_update_fails_if_not_enabled)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        EXPECT_CALL(left_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(right_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(arduino_, init_encoders(10000, 1, -1));
        EXPECT_CALL(left_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(left_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(right_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(right_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        azhoo.configure(config_);

        // Act
        bool success = azhoo.update();
        
        // Assert
        ASSERT_FALSE(success);
}

TEST_F(azhoo_tests, test_update_and_set_target_fail_if_disabled)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000));
        EXPECT_CALL(left_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(right_pwm_, amplitude())
                .WillOnce(Return(500));
        EXPECT_CALL(arduino_, init_encoders(10000, 1, -1));
        EXPECT_CALL(left_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(left_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(right_encoder_, get_position())
                .WillOnce(Return(0));
        EXPECT_CALL(right_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        azhoo.configure(config_);
        azhoo.enable();
        azhoo.disable();

        // Act
        bool set_speed = azhoo.set_target_speeds(100, 100);
        bool updated = azhoo.update();
        
        // Assert
        ASSERT_FALSE(set_speed);
        ASSERT_FALSE(updated);
}

TEST_F(azhoo_tests, test_update)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));        
        EXPECT_CALL(arduino_, init_encoders(10000, 1, -1));
        EXPECT_CALL(left_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(right_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(left_pwm_, amplitude())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(right_pwm_, amplitude())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(left_pwm_, center())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(right_pwm_, center())
                .WillRepeatedly(Return(1000));
        
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000))
                .WillOnce(Return(1000 + Azhoo::kDefaultUpdateInterval));
        
        EXPECT_CALL(left_encoder_, get_position())
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        
        EXPECT_CALL(right_encoder_, get_position())
                .WillOnce(Return(0))
                .WillOnce(Return(0));

        EXPECT_CALL(left_pwm_, set(1008));
        EXPECT_CALL(right_pwm_, set(1008));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);

        azhoo.setup();
        azhoo.configure(config_);
        azhoo.enable();
        azhoo.set_target_speeds(100, 100);
        
        // Act
        bool updated = azhoo.update();
        
        // Assert
        ASSERT_TRUE(azhoo.is_enabled());
        ASSERT_TRUE(updated);
        
        ASSERT_EQ(azhoo.left_speed_envelope_.target_, 100);
        ASSERT_EQ(azhoo.left_speed_envelope_.increment_, 20);
        ASSERT_EQ(azhoo.left_speed_envelope_.current_, 20);
        
        ASSERT_EQ(azhoo.right_speed_envelope_.target_, 100);
        ASSERT_EQ(azhoo.right_speed_envelope_.increment_, 20);
        ASSERT_EQ(azhoo.right_speed_envelope_.current_, 20);
        
        ASSERT_EQ(azhoo.left_controller_.delta_coeff_, 200);
        ASSERT_EQ(azhoo.left_controller_.delta_, 0);
        ASSERT_EQ(azhoo.left_controller_.delta_target_, 4);
        ASSERT_EQ(azhoo.left_controller_.error_, 4);
        ASSERT_EQ(azhoo.left_controller_.sum_, 4);
        ASSERT_EQ(azhoo.left_controller_.p_, 4);
        ASSERT_EQ(azhoo.left_controller_.i_, 4);
        ASSERT_EQ(azhoo.left_controller_.out_, 8);
        ASSERT_EQ(azhoo.left_controller_.pulsewidth_, 1008);
        
        ASSERT_EQ(azhoo.right_controller_.delta_coeff_, 200);
        ASSERT_EQ(azhoo.right_controller_.delta_, 0);
        ASSERT_EQ(azhoo.right_controller_.delta_target_, 4);
        ASSERT_EQ(azhoo.right_controller_.error_, 4);
        ASSERT_EQ(azhoo.right_controller_.sum_, 4);
        ASSERT_EQ(azhoo.right_controller_.p_, 4);
        ASSERT_EQ(azhoo.right_controller_.i_, 4);
        ASSERT_EQ(azhoo.right_controller_.out_, 8);
        ASSERT_EQ(azhoo.right_controller_.pulsewidth_, 1008);
}

TEST_F(azhoo_tests, test_update_default_to_zero_speed)
{
        // Arrange
        EXPECT_CALL(arduino_, left_encoder())
                .WillOnce(ReturnRef(left_encoder_));
        EXPECT_CALL(arduino_, right_encoder())
                .WillOnce(ReturnRef(right_encoder_));
        EXPECT_CALL(arduino_, left_pwm())
                .WillOnce(ReturnRef(left_pwm_));
        EXPECT_CALL(arduino_, right_pwm())
                .WillOnce(ReturnRef(right_pwm_));        
        EXPECT_CALL(arduino_, init_encoders(10000, 1, -1));
        EXPECT_CALL(left_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(right_encoder_, positions_per_revolution())
                .WillOnce(Return(10000));
        EXPECT_CALL(left_pwm_, amplitude())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(right_pwm_, amplitude())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(left_pwm_, center())
                .WillRepeatedly(Return(1000));
        EXPECT_CALL(right_pwm_, center())
                .WillRepeatedly(Return(1000));
        
        EXPECT_CALL(arduino_, milliseconds())
                .WillOnce(Return(1000))
                .WillOnce(Return(1000 + Azhoo::kDefaultUpdateInterval));
        
        EXPECT_CALL(left_encoder_, get_position())
                .WillOnce(Return(0))
                .WillOnce(Return(0));
        
        EXPECT_CALL(right_encoder_, get_position())
                .WillOnce(Return(0))
                .WillOnce(Return(0));

        EXPECT_CALL(left_pwm_, set(1000));
        EXPECT_CALL(right_pwm_, set(1000));
        
        Azhoo azhoo(arduino_, Azhoo::kDefaultUpdateInterval);
        azhoo.setup();
        azhoo.configure(config_);
        azhoo.enable();
        
        // Act
        bool updated = azhoo.update();
        
        // Assert
        ASSERT_TRUE(azhoo.is_enabled());
        ASSERT_TRUE(updated);
        
        ASSERT_EQ(azhoo.left_speed_envelope_.target_, 0);
        ASSERT_EQ(azhoo.left_speed_envelope_.current_, 0);
        
        ASSERT_EQ(azhoo.right_speed_envelope_.target_, 0);
        ASSERT_EQ(azhoo.right_speed_envelope_.current_, 0);
        
        ASSERT_EQ(azhoo.left_controller_.delta_, 0);
        ASSERT_EQ(azhoo.left_controller_.delta_target_, 0);
        ASSERT_EQ(azhoo.left_controller_.error_, 0);
        ASSERT_EQ(azhoo.left_controller_.sum_, 0);
        ASSERT_EQ(azhoo.left_controller_.p_, 0);
        ASSERT_EQ(azhoo.left_controller_.i_, 0);
        ASSERT_EQ(azhoo.left_controller_.out_, 0);
        ASSERT_EQ(azhoo.left_controller_.pulsewidth_, 1000);
        
        ASSERT_EQ(azhoo.right_controller_.delta_, 0);
        ASSERT_EQ(azhoo.right_controller_.delta_target_, 0);
        ASSERT_EQ(azhoo.right_controller_.error_, 0);
        ASSERT_EQ(azhoo.right_controller_.sum_, 0);
        ASSERT_EQ(azhoo.right_controller_.p_, 0);
        ASSERT_EQ(azhoo.right_controller_.i_, 0);
        ASSERT_EQ(azhoo.right_controller_.out_, 0);
        ASSERT_EQ(azhoo.right_controller_.pulsewidth_, 1000);
}

