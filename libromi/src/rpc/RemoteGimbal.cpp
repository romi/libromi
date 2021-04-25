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
#include "rpc/RemoteGimbal.h"
#include "rpc/MethodsGimbal.h"
#include "rpc/MethodsPowerDevice.h"
#include "rpc/MethodsActivity.h"

namespace romi {
        
        RemoteGimbal::RemoteGimbal(std::unique_ptr<IRPCClient> &client)
                : RemoteStub(client)
        {
        }
        
        bool RemoteGimbal::moveto(double angle)
        {
                r_debug("RemoteGimbal::moveto");
                JsonCpp params = JsonCpp::construct("{ \"%s\": %.3f}",
                                                    MethodsGimbal::angle_param,
                                                    angle);
                return execute_with_params(MethodsGimbal::moveto, params);
        }
        
        bool RemoteGimbal::get_position(double& angle)
        {
                r_debug("RemoteGimbal::get_position");
                bool success = false;
                JsonCpp result;
                if (execute_with_result(MethodsGimbal::get_position, result)) {
                        angle = result.num(MethodsGimbal::angle_result);
                        success = true;
                }
                return success;
        }

        bool RemoteGimbal::pause_activity()
        {
                r_debug("RemoteGimbal::stop_activity");
                return execute_simple_request(MethodsActivity::activity_pause);
        }

        bool RemoteGimbal::continue_activity()
        {
                r_debug("RemoteGimbal::continue_activity");
                return execute_simple_request(MethodsActivity::activity_continue);
        }

        bool RemoteGimbal::reset_activity()
        {
                r_debug("RemoteGimbal::reset");
                return execute_simple_request(MethodsActivity::activity_reset);
        }

        bool RemoteGimbal::power_up()
        {
                r_debug("RemoteGimbal::power_up");
                return execute_simple_request(MethodsPowerDevice::power_up);
        }
        
        bool RemoteGimbal::power_down()
        {
                r_debug("RemoteGimbal::power_down");
                return execute_simple_request(MethodsPowerDevice::power_down);
        }
        
        bool RemoteGimbal::stand_by()
        {
                r_debug("RemoteGimbal::stand_by");
                return execute_simple_request(MethodsPowerDevice::stand_by);
        }
        
        bool RemoteGimbal::wake_up()
        {
                r_debug("RemoteGimbal::wake_up");
                return execute_simple_request(MethodsPowerDevice::wake_up);
        }
}
