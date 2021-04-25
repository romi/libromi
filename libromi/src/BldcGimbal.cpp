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
#include <r.h>
#include "BldcGimbal.h"

namespace romi {
        
        BldcGimbal::BldcGimbal(IRomiSerialClient& serial)
                : serial_(serial)
        {
        }

        int BldcGimbal::angle_to_arg(double angle)
        {
                return (int) (clamp(angle) * 10.0);
        }

        double BldcGimbal::arg_to_angle(double arg)
        {
                return arg / 10.0;
        }
        
        double BldcGimbal::clamp(double angle)
        {
                angle = std::fmod(angle, 360.0);
                if (angle < 0.0)
                        angle += 360.0;
                return angle;
        }

        bool BldcGimbal::moveto(double angle)
        {
                char command[64];
                snprintf(command, sizeof(command), "M[%d]", angle_to_arg(angle));
                
                JsonCpp response;
                serial_.send(command, response);
                
                bool success = (response.num(0) == 0);
                if (!success) {
                        r_err("BldcGimbal::moveto: %s", response.str(1));
                }
                return success;
        }
                        
        bool BldcGimbal::get_position(double& angle)
        {
                JsonCpp response;
                serial_.send("s", response);
                
                bool success = (response.num(0) == 0);
                if (success) {
                        angle = arg_to_angle(response.num(1));
                } else {
                        r_err("BldcGimbal::get_position: %s", response.str(1));
                }
                return success;
        }

        bool BldcGimbal::pause_activity()
        {
                return true;
        }

        bool BldcGimbal::continue_activity()
        {
                return true;
        }

        bool BldcGimbal::reset_activity()
        {
                return true;
        }

        bool BldcGimbal::power_up()
        {
                return true;
        }

        bool BldcGimbal::power_down()
        {
                return true;
        }

        bool BldcGimbal::stand_by()
        {
                return true;
        }

        bool BldcGimbal::wake_up()
        {
                return true;
        }
}
