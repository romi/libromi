#include <string>
#include <iostream>
#include <thread>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <oquam/Helix.h>
#include <oquam/Oquam.h>
#include <oquam/StepperSettings.h>
#include <mock_cnccontroller.h>
#include <mock_session.h>


using namespace std;
using namespace testing;
using namespace romi;

class oquam_helix_tests : public ::testing::Test
{
protected:
        int32_t pos_[3];
        
	oquam_helix_tests() {
                for (int i = 0; i < 3; i++)
                        pos_[i] = 0;
        }

	~oquam_helix_tests() override = default;
        
	void SetUp() override {
        }

	void TearDown() override {}

public:
        
        bool get_position(int32_t *pos) {
                for (int i = 0; i < 3; i++)
                        pos[i] = pos_[i];
                return true;
        }
        
        bool moveto(int16_t millis, int16_t x, int16_t y, int16_t z) {
                (void) millis;
                pos_[0] = x;
                pos_[1] = y;
                pos_[2] = z;
                return true;
        }
        
        bool move(int16_t millis, int16_t x, int16_t y, int16_t z) {
                (void) millis;
                pos_[0] += x;
                pos_[1] += y;
                pos_[2] += z;
                return true;
        }

};

TEST_F(oquam_helix_tests, test_zero_travel)
{
        // Arrange

        // Act
        Helix helix(0.0, 0.0,      // x0, y0
                    0.0, 0.0, 0.0, // xc, yc, alpha
                    0.0, 0.0,      // z0, z1
                    1.0, 1.0,      // vxy, axy
                    1.0, 1.0);     // vz, az

        // Assert
        v3 v;
        helix.get_position_at_u(0.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
        
        helix.get_position_at_u(0.5);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
        
        helix.get_position_at_u(1.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
}

TEST_F(oquam_helix_tests, test_travel_one_up)
{
        // Arrange

        // Act
        Helix helix(0.0, 0.0,
                    0.0, 0.0, 0.0,
                    0.0, 1.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;
        double u;
        
        p = helix.get_position_at_u(0.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(0.5);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.5);
        
        p = helix.get_position_at_u(1.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 1.0);

        //
        u = helix.get_u_position_at(0.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        u = helix.get_u_position_at(1.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.5);
        
        u = helix.get_u_position_at(2.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 1.0);

        //
        p = helix.get_position_at(0.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at(1.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.5);
        
        p = helix.get_position_at(2.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 1.0);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(1.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 1.0);
        
        v = helix.get_speed_at(2.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
}

TEST_F(oquam_helix_tests, test_travel_one_down)
{
        // Arrange

        // Act
        Helix helix(0.0, 0.0,
                    0.0, 0.0, 0.0,
                    0.0, -1.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;

        //
        p = helix.get_position_at(0.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at(1.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), -0.5);
        
        p = helix.get_position_at(2.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), -1.0);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(1.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), -1.0);
        
        v = helix.get_speed_at(2.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
}


TEST_F(oquam_helix_tests, test_travel_two_up)
{
        // Arrange

        // Act
        Helix helix(0.0, 0.0,
                    0.0, 0.0, 0.0,
                    0.0, 2.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;
        double u;

        //
        u = helix.get_u_position_at(0.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        u = helix.get_u_position_at(1.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.5);
        
        u = helix.get_u_position_at(2.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 1.5);
        
        u = helix.get_u_position_at(3.0);
        p = helix.get_position_at_u(u);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 2.0);

        //
        p = helix.get_position_at(0.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at(1.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.5);
        
        p = helix.get_position_at(2.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 1.5);
        
        p = helix.get_position_at(3.0);
        ASSERT_EQ(p.x(), 0.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 2.0);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(1.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 1.0);
        
        v = helix.get_speed_at(2.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 1.0);
        
        v = helix.get_speed_at(3.0);
        ASSERT_EQ(v.x(), 0.0);
        ASSERT_EQ(v.y(), 0.0);
        ASSERT_EQ(v.z(), 0.0);
}

TEST_F(oquam_helix_tests, test_travel_semi_circle_ccw)
{
        // Arrange

        // Act
        Helix helix(1.0, 0.0,
                    0.0, 0.0, M_PI,
                    0.0, 0.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;
        // double u;
        
        p = helix.get_position_at_u(0.0);
        ASSERT_EQ(p.x(), 1.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(0.5);
        ASSERT_NEAR(p.x(), 0.0, 0.000001);
        ASSERT_NEAR(p.y(), 1.0, 0.000001);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(1.0);
        ASSERT_NEAR(p.x(), -1.0, 0.000001);
        ASSERT_NEAR(p.y(), 0.0, 0.000001);
        ASSERT_EQ(p.z(), 0.0);

        // The path length is PI. The duration is: 1 s for an
        // acceleration over a path length of 0.5, similar for
        // deceleration, and a duration of PI-1 for the traveling of
        // the remaining path, or a total duration of PI+1.
        double duration = M_PI + 1.0;
        ASSERT_NEAR(helix.get_duration(), duration, 0.00001);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);

        double alpha = 0.5;
        v = helix.get_speed_at(1.0);
        ASSERT_NEAR(v.x(), -sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), cos(alpha), 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(duration/2.0);
        ASSERT_NEAR(v.x(), -1.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        alpha = M_PI - 0.5;
        v = helix.get_speed_at(duration - 1.0);
        ASSERT_NEAR(v.x(), -sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), cos(alpha), 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(duration);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
}

TEST_F(oquam_helix_tests, test_travel_semi_circle_cw)
{
        // Arrange

        // Act
        Helix helix(1.0, 0.0,
                    0.0, 0.0, -M_PI,
                    0.0, 0.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;
        // double u;
        
        p = helix.get_position_at_u(0.0);
        ASSERT_EQ(p.x(), 1.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(0.5);
        ASSERT_NEAR(p.x(), 0.0, 0.000001);
        ASSERT_NEAR(p.y(), -1.0, 0.000001);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(1.0);
        ASSERT_NEAR(p.x(), -1.0, 0.000001);
        ASSERT_NEAR(p.y(), 0.0, 0.000001);
        ASSERT_EQ(p.z(), 0.0);

        // The path length is PI. The duration is: 1 s for an
        // acceleration over a path length of 0.5, similar for
        // deceleration, and a duration of PI-1 for the traveling of
        // the remaining path, or a total duration of PI+1.
        double duration = M_PI + 1.0;
        ASSERT_NEAR(helix.get_duration(), duration, 0.00001);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);

        double alpha = -0.5;
        v = helix.get_speed_at(1.0);
        ASSERT_NEAR(v.x(), sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), -cos(alpha), 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(duration/2.0);
        ASSERT_NEAR(v.x(), -1.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        alpha = -(M_PI - 0.5);
        v = helix.get_speed_at(duration - 1.0);
        ASSERT_NEAR(v.x(), sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), -cos(alpha), 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
        
        v = helix.get_speed_at(duration);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);
}

TEST_F(oquam_helix_tests, test_travel_semi_circle_and_one_up)
{
        // Arrange

        // Act
        Helix helix(1.0, 0.0,
                    0.0, 0.0, M_PI,
                    0.0, 1.0,
                    1.0, 1.0,
                    1.0, 1.0);

        // Assert
        v3 p;
        v3 v;
        // double u;
        
        p = helix.get_position_at_u(0.0);
        ASSERT_EQ(p.x(), 1.0);
        ASSERT_EQ(p.y(), 0.0);
        ASSERT_EQ(p.z(), 0.0);
        
        p = helix.get_position_at_u(0.5);
        ASSERT_NEAR(p.x(), 0.0, 0.000001);
        ASSERT_NEAR(p.y(), 1.0, 0.000001);
        ASSERT_EQ(p.z(), 0.5);
        
        p = helix.get_position_at_u(1.0);
        ASSERT_NEAR(p.x(), -1.0, 0.000001);
        ASSERT_NEAR(p.y(), 0.0, 0.000001);
        ASSERT_EQ(p.z(), 1.0);

        // The path length is PI. The duration is: 1 s for an
        // acceleration over a path length of 0.5, similar for
        // deceleration, and a duration of PI-1 for the traveling of
        // the remaining path, or a total duration of PI+1.
        double duration = M_PI + 1.0;
        ASSERT_NEAR(helix.get_duration(), duration, 0.00001);

        //
        v = helix.get_speed_at(0.0);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_EQ(v.z(), 0.0);

        /*
          Traveling speed on z-axis (after 1 second): 
          
          distance traveled during acceleration:
          len_a = 0.5*a*t²    (1)

          Because proportional to xy travel: 0.5 traveled for a total
          length of PI:
          
          path length PI  -> position 0.5      (xy-plane)
          path length 1   -> position 0.5/PI   (z-axis)
          
          len_a = 0.5/PI      (2)

          => az = 1/PI   (t = 1)
          => vz = az * t = 1/PI  
         */
        double alpha = 0.5;
        v = helix.get_speed_at(1.0);
        ASSERT_NEAR(v.x(), -sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), cos(alpha), 0.0000001);
        ASSERT_NEAR(v.z(), 1.0 / M_PI, 0.0000001);
        
        v = helix.get_speed_at(duration/2.0);
        ASSERT_NEAR(v.x(), -1.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_NEAR(v.z(), 1.0 / M_PI, 0.0000001);
        
        alpha = M_PI - 0.5;
        v = helix.get_speed_at(duration - 1.0);
        ASSERT_NEAR(v.x(), -sin(alpha), 0.0000001);
        ASSERT_NEAR(v.y(), cos(alpha), 0.0000001);
        ASSERT_NEAR(v.z(), 1.0 / M_PI, 0.0000001);
        
        v = helix.get_speed_at(duration);
        ASSERT_NEAR(v.x(), 0.0, 0.0000001);
        ASSERT_NEAR(v.y(), 0.0, 0.0000001);
        ASSERT_NEAR(v.z(), 0.0, 0.0000001);
}


TEST_F(oquam_helix_tests, test_helix_full_circle)
{
        // Arrange
        nlohmann::json config = nlohmann::json::parse(
                "{"
                "    \"oquam\": {"
                "        \"cnc-range\": [[0, 0.750000], [0, 0.750000], [-100000, 100000]], "
                "        \"path-slice-duration\": 0.020000, "
                "        \"path-maximum-deviation\": 0.010000, "
                "        \"controller-classname\": \"stepper-controller\", "
                "        \"stepper-settings\": { "
                "            \"displacement-per-revolution\": [0.040000, 0.040000, 6.283185307179586], "
                "            \"gears-ratio\": [1, 1, 1], "
                "            \"maximum-acceleration\": [0.200000, 0.200000, 3.0000], "
                "            \"maximum-rpm\": [300, 300, 300], "
                "            \"microsteps\": [8, 8, 2], "
                "            \"steps-per-revolution\": [200, 200, 200] "
                "        } "
                "    }"
                "}");
        romi::AxisIndex homing[3] = { romi::kAxisX, romi::kAxisY, romi::kNoAxis };
        nlohmann::json range_data = config.at("oquam").at("cnc-range");
        romi::CNCRange range(range_data);

        nlohmann::json stepper_data = config.at("oquam").at("stepper-settings");
        romi::StepperSettings stepper_settings(stepper_data);
        
        double slice_duration = (double) config["oquam"]["path-slice-duration"];
        double maximum_deviation = (double) config["oquam"]["path-maximum-deviation"];
        double max_steps_per_block = 32000.0; // Should be less than 2^15/2-1
        double max_slice_duration = stepper_settings.compute_minimum_duration(max_steps_per_block);
                
        romi::OquamSettings oquam_settings(range,
                                           stepper_settings.maximum_speed,
                                           stepper_settings.maximum_acceleration,
                                           stepper_settings.steps_per_meter,
                                           maximum_deviation,
                                           slice_duration,
                                           max_slice_duration,
                                           homing);
        MockCNCController controller;
        MockSession session;


        EXPECT_CALL(controller, set_homing_axes(_,_,_))
                .WillRepeatedly(Return(true));
        EXPECT_CALL(controller, spindle(0))
                .WillOnce(Return(true));
        EXPECT_CALL(controller, get_position(_))
                .WillRepeatedly(Invoke(this, &oquam_helix_tests::get_position));
        EXPECT_CALL(controller, moveto(_,_,_,_))
                .WillRepeatedly(Invoke(this, &oquam_helix_tests::moveto));
        EXPECT_CALL(controller, move(_,_,_,_))
                .WillRepeatedly(Invoke(this, &oquam_helix_tests::move));
        EXPECT_CALL(controller, synchronize(_))
                .WillRepeatedly(Return(true));

        romi::Oquam oquam(controller, oquam_settings, session);

        // Act
        oquam.moveto(0.0, 0.375, 0.0, 0.5);
        oquam.helix(0.375, 0.375, -2 * M_PI, -2 * M_PI, 1.0);
                    
        // Assert
        ASSERT_EQ(pos_[0], 0);
        ASSERT_NEAR(pos_[1], 15000, 1); // (0.375/0.04)*(8*200)
        ASSERT_EQ(pos_[2], -400);  // (-2*pi/2*pi)*(2*200)
        
}
