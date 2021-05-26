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
#ifndef __ROMI_MOTORDRIVER_H
#define __ROMI_MOTORDRIVER_H

namespace romi {
        
        class IMotorDriver
        {
        public:
                
                virtual ~IMotorDriver() = default;

                /** The left and right speed are relative speeds. They
                 * must have a value between -1 and 1, and indicate
                 * the fraction of the maximum allowed speed. */
                virtual bool moveat(double left, double right) = 0;

                /** Stop as quick as possible. */
                virtual bool stop() = 0;

                /** Returns the values of the encoders. The timestamp
                 * is in seconds. */
                virtual bool get_encoder_values(double& left, double& right,
                                                double& timestamp) = 0;

                enum Axis { kLeftWheel = 0, kRightWheel = 1 };
                
                virtual bool get_pid_values(Axis axis,
                                            double& target_speed,
                                            double& measured_speed,
                                            double& pid_output,
                                            double& pid_error_p,
                                            double& pid_error_i,
                                            double& pid_error_d,
                                            double& controller_input) = 0;
        };
}

#endif // __ROMI_MOTORDRIVER_H
