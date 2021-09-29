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
#include "rover/DifferentialSteering.h"
#include "api/DataLogAccessor.h"

namespace romi {
        
        static const std::string kLeftSpeedName = "navigation-left-speed";
        static const std::string kRightSpeedName = "navigation-right-speed";
        static const std::string kCorrectionName = "navigation-correction";
        
        DifferentialSteering::DifferentialSteering(IMotorDriver& driver,
                                                   NavigationSettings& settings)
                : motors_(driver),
                  settings_(settings),
                  left_speed_(0.0),
                  right_speed_(0.0)
        {
        }

        bool DifferentialSteering::stop()
        {
                bool success = motors_.stop();
                if (success) {
                        left_speed_ = 0.0;
                        right_speed_ = 0.0;
                }
                return success;
        }

        bool DifferentialSteering::drive(double speed, SteeringData steering)
        {
                bool success;
                if (steering.type_ == kStraight)
                        success = forward(speed);
                else
                        success = turn(speed, steering.radius_);
                return success;
        }
                
        bool DifferentialSteering::forward(double speed)
        {
                if (speed > settings_.maximum_speed)
                        speed = settings_.maximum_speed;
                else if (speed < -settings_.maximum_speed)
                        speed = -settings_.maximum_speed;

                log_data(kCorrectionName, 0.0);
                log_data(kLeftSpeedName, speed);
                log_data(kRightSpeedName, speed);
                
                return moveat(speed, speed);
        }
        
        bool DifferentialSteering::turn(double speed, double radius)
        {
                double delta = settings_.wheeltrack / (2.0 * radius);
                double left_speed = speed * (1 - delta);
                double right_speed = speed * (1 + delta);

                if (left_speed > settings_.maximum_speed) {
                        double scale = settings_.maximum_speed / left_speed;
                        left_speed *= scale;
                        right_speed *= scale;
                } else if (right_speed > settings_.maximum_speed) {
                        double scale = settings_.maximum_speed / right_speed;
                        left_speed *= scale;
                        right_speed *= scale;
                } else if (left_speed < -settings_.maximum_speed) {
                        double scale = std::abs(settings_.maximum_speed / left_speed);
                        left_speed *= scale;
                        right_speed *= scale;
                } else if (right_speed < -settings_.maximum_speed) {
                        double scale = std::abs(settings_.maximum_speed / right_speed);
                        left_speed *= scale;
                        right_speed *= scale;
                } 

                log_data(kCorrectionName, delta);
                log_data(kLeftSpeedName, left_speed);
                log_data(kRightSpeedName, right_speed);

                return moveat(left_speed, right_speed);
        }

        bool DifferentialSteering::moveat(double left, double right)
        {
                bool success = true;
                
                if (left != left_speed_
                    || right != right_speed_) {
                        
                        double left_angular_speed = settings_.to_angular_speed(left);
                        double right_angular_speed = settings_.to_angular_speed(right);
                        
                        success = motors_.moveat(left_angular_speed, right_angular_speed);

                        if (success) {
                                left_speed_ = left;
                                right_speed_ = right;
                        }
                }
                
                return success;
        }

        bool DifferentialSteering::enable()
        {
                return motors_.enable_controller();
        }

        bool DifferentialSteering::disable()
        {
                return motors_.disable_controller();
        }

        bool DifferentialSteering::initialise() {
            return true;
        }
}
