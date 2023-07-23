/*
  romi-rover

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  romi-rover is collection of applications for the Romi Rover.

  romi-rover is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

*/
#include <cmath>
#include "util/Logger.h"
#include "hal/BldcGimbalI2C.h"

namespace romi {
        
        BldcGimbalI2C::BldcGimbalI2C(std::unique_ptr<II2C>& bus)
                : bus_(std::move(bus)),
                  power_(kDefaultPower)
        {
                set_zero();
                reset();
                motor_wake();
                set_motor_power(40.0);
                //set_motor_position(0.0);
		  //set_angle(0.0);
		  //set_follow(true);
        }

        int BldcGimbalI2C::angle_to_arg(double angle)
        {
                return (int) (clamp(angle) * 10.0);
        }

        double BldcGimbalI2C::arg_to_angle(double arg)
        {
                return arg / 10.0;
        }
        
        double BldcGimbalI2C::clamp(double angle)
        {
                angle = std::fmod(angle, 360.0);
                if (angle < 0.0)
                        angle += 360.0;
                return angle;
        }
        
        bool BldcGimbalI2C::get_range(IRange& range)
        {
                v3 min(-360.0, 0.0, 0.0);
                v3 max(360.0, 0.0, 0.0);
                range.init(min, max);
                return true;
        }

        bool BldcGimbalI2C::moveto(double phi_x, double phi_y, double phi_z,
                                   double relative_speed)
        {
                (void) phi_y;
                (void) phi_z;
                (void) relative_speed;
                //set_angle(phi_x);
		set_motor_position(phi_x);
                return true;
        }

        // bool BldcGimbalI2C::moveat(double wx, double wy, double wz)
        // {
        //         (void) wx;
        //         (void) wy;
        //         (void) wz;
        //         return true;
        // }

        bool BldcGimbalI2C::get_position(v3& position)
        {
                position.set(0.0);
                position.x(get_angle());
                return true;
        }
        
        // bool BldcGimbalI2C::set_angle(double angle)
        // {
        //         (void) angle;
        //         return true;
        // }

        int32_t BldcGimbalI2C::scale_up(double value)
        {
                return (int32_t) (value * 10000.0);
        }
        
        double BldcGimbalI2C::scale_down(int32_t value)
        {
                return (double) value / 10000.0;
        }
        
        void BldcGimbalI2C::set_follow(bool follow)
        {
                int32_t value = follow? 1 : 0;
                bus_->set(kFollow, value);
        }
        
        double BldcGimbalI2C::get_angle()
        {
                int32_t value = bus_->get(kAngle);
                double angle = scale_down(value);
                r_debug("BldcGimbalI2C::get_angle: %.2f", angle);
                return angle;
        }
        
        void BldcGimbalI2C::set_angle(double angle)
        {
 	        r_debug("BldcGimbalI2C::set_angle: %.2f", angle);
                int32_t value = scale_up(angle);
                bus_->set(kAngle, value);
        }
        
        double BldcGimbalI2C::get_zero_offset()
        {
                int32_t value = bus_->get(kZeroOffset);
                return scale_down(value);
        }
        
        void BldcGimbalI2C::set_zero_offset(double offset)
        {
                int32_t value = scale_up(offset);
                bus_->set(kZeroOffset, value);
        }
        
        void BldcGimbalI2C::set_zero()
        {
                bus_->set(kZero, 0);
        }
        
        double BldcGimbalI2C::get_max()
        {
                int32_t value = bus_->get(kMax);
                return scale_down(value);
        }
        
        void BldcGimbalI2C::set_max(double max)
        {
                int32_t value = scale_up(max);
                bus_->set(kMax, value);
        }
        
        void BldcGimbalI2C::motor_sleep()
        {
                bus_->set(kMotorSleep, 1);
        }
        
        void BldcGimbalI2C::motor_wake()
        {
                bus_->set(kMotorSleep, 0);
                set_motor_position(0.0);
        }
        
        double BldcGimbalI2C::get_motor_power()
        {
                return (double) bus_->get(kMotorPower);
        }
        
        void BldcGimbalI2C::set_motor_power(double power)
        {
                int32_t value = (int32_t) power;
                bus_->set(kMotorPower, value);
        }
        
        double BldcGimbalI2C::get_motor_position()
        {
                int32_t value = bus_->get(kMotorPosition);
                return scale_down(value);
        }
        
        void BldcGimbalI2C::set_motor_position(double position)
        {
                int32_t value = scale_up(position);
                bus_->set(kMotorPosition, value);
        }
        
        double BldcGimbalI2C::get_kp()
        {
                int32_t value = bus_->get(kKp);
                return scale_down(value);
        }
        
        void BldcGimbalI2C::set_kp(double kp)
        {
                int32_t value = scale_up(kp);
                bus_->set(kKp, value);
        }
        
        double BldcGimbalI2C::get_max_accel()
        {
                int32_t value = bus_->get(kMaxAccelation);
                return scale_down(value);
        }
        
        void BldcGimbalI2C::set_max_accel(double acceleration)
        {
                int32_t value = scale_up(acceleration);
                bus_->set(kMaxAccelation, value);
        }
        
        void BldcGimbalI2C::reset()
        {
                bus_->set(kReset, 1);
        }
        
        bool BldcGimbalI2C::pause_activity()
        {
                r_err("BldcGimbalI2C::pause_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::pause_activity: Not implemented");
        }

        bool BldcGimbalI2C::continue_activity()
        {
                r_err("BldcGimbalI2C::continue_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::continue_activity: Not implemented");
        }

        bool BldcGimbalI2C::reset_activity()
        {
                r_err("BldcGimbalI2C::reset_activity: Not implemented");
                throw std::runtime_error("BldcGimbalI2C::reset_activity: Not implemented");
        }

        bool BldcGimbalI2C::power_up()
        {
                return true; // FIXME
        }

        bool BldcGimbalI2C::power_down()
        {
                return true; // FIXME
        }

        bool BldcGimbalI2C::homing()
        {
                return true; // false?
        }

        bool BldcGimbalI2C::stand_by()
        {
                return power_down();
        }

        bool BldcGimbalI2C::wake_up()
        {
                return power_up();
        }
}
