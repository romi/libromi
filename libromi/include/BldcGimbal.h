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
#ifndef __ROMI_BLDC_GIMBAL_H
#define __ROMI_BLDC_GIMBAL_H

#include "api/IGimbal.h"
#include "IRomiSerialClient.h"

namespace romi {
        
        class BldcGimbal : public IGimbal
        {
        protected:
                IRomiSerialClient& serial_;

                double clamp(double angle_in_degrees);
                int angle_to_arg(double angle);
                double arg_to_angle(double arg);
                
        public:
                BldcGimbal(IRomiSerialClient& serial);
                virtual ~BldcGimbal() = default;
                                
                // IGimbal
                bool moveto(double angle_in_degrees) override;
                bool get_position(double& value) override;

                // IActivity
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // IPowerDevice
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_BLDC_GIMBAL_H
