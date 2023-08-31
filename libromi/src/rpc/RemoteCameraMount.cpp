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

#include "util/Logger.h"
#include "rpc/RemoteCameraMount.h"
#include "rpc/MethodsCameraMount.h"
#include "rpc/MethodsPowerDevice.h"
#include "rpc/MethodsActivity.h"

namespace romi {
        
        RemoteCameraMount::RemoteCameraMount(std::unique_ptr<rcom::IRPCClient> &client)
                : RemoteStub(client)
        {
        }
        
        bool RemoteCameraMount::moveto(double x, double y, double z,
                                       double ax, double ay, double az,
                                       double relative_speed)
        {
                r_debug("RemoteCameraMount::moveto");
                nlohmann::json params{
                        {MethodsCameraMount::kXParam, x},
                        {MethodsCameraMount::kYParam, y},
                        {MethodsCameraMount::kZParam, z},
                        {MethodsCameraMount::kAngleXParam, ax},
                        {MethodsCameraMount::kAngleYParam, ay},
                        {MethodsCameraMount::kAngleZParam, az},
                        {MethodsCameraMount::kSpeedParam, relative_speed},
                };
                return execute_with_params(MethodsCameraMount::kMoveto, params);
        }
        
        bool RemoteCameraMount::get_position(v3& xyz, v3& angles)
        {
                r_debug("RemoteCameraMount::get_position");
                bool success = false;
                nlohmann::json result;
                if (execute_with_result(MethodsCameraMount::kGetPosition, result)) {
                        xyz.x(result[MethodsCameraMount::kPositionX]);
                        xyz.y(result[MethodsCameraMount::kPositionY]);
                        xyz.z(result[MethodsCameraMount::kPositionZ]);
                        angles.x(result[MethodsCameraMount::kPositionAngleX]);
                        angles.y(result[MethodsCameraMount::kPositionAngleY]);
                        angles.z(result[MethodsCameraMount::kPositionAngleZ]);
                        success = true;
                }
                return success;
        }

        bool RemoteCameraMount::get_range(CNCRange &xyz, IRange &angles)
        {
                r_debug("RemoteCameraMount::get_range");

                bool success = false;
                nlohmann::json result;

                try {
                        if (execute_with_result(MethodsCameraMount::kGetRange, result)) {
                                xyz.init(result[MethodsCameraMount::kXYZRange]);
                                angles.init(result[MethodsCameraMount::kAnglesRange]);
                                success = true;
                        }
                        
                } catch (nlohmann::json::exception& je) {
                        r_err("RemoteCNC::get_range failed: %s", je.what());
                }

                return success;
        }

        bool RemoteCameraMount::homing()
        {
                r_debug("RemoteCameraMount::homing");
                return execute_simple_request(MethodsCameraMount::kHoming);
        }

        uint8_t RemoteCameraMount::count_relays()
        {
                return 1;
        }
        
        bool RemoteCameraMount::set_relay(uint8_t index, bool value)
        {
                r_debug("RemoteCameraMount::set_relay");
                nlohmann::json params{
                        {MethodsCameraMount::kIndex, index},
                        {MethodsCameraMount::kValue, (double) value}
                };
                return execute_with_params(MethodsCameraMount::kSetRelay, params);
        }
        
        bool RemoteCameraMount::pause_activity()
        {
                r_debug("RemoteCameraMount::stop_activity");
                return execute_simple_request(MethodsActivity::activity_pause);
        }

        bool RemoteCameraMount::continue_activity()
        {
                r_debug("RemoteCameraMount::continue_activity");
                return execute_simple_request(MethodsActivity::activity_continue);
        }

        bool RemoteCameraMount::reset_activity()
        {
                r_debug("RemoteCameraMount::reset");
                return execute_simple_request(MethodsActivity::activity_reset);
        }

        bool RemoteCameraMount::power_up()
        {
                r_debug("RemoteCameraMount::power_up");
                return execute_simple_request(MethodsPowerDevice::power_up);
        }
        
        bool RemoteCameraMount::power_down()
        {
                r_debug("RemoteCameraMount::power_down");
                return execute_simple_request(MethodsPowerDevice::power_down);
        }
        
        bool RemoteCameraMount::stand_by()
        {
                r_debug("RemoteCameraMount::stand_by");
                return execute_simple_request(MethodsPowerDevice::stand_by);
        }
        
        bool RemoteCameraMount::wake_up()
        {
                r_debug("RemoteCameraMount::wake_up");
                return execute_simple_request(MethodsPowerDevice::wake_up);
        }
}
