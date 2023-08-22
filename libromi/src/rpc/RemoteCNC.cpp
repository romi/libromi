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
#include "rpc/RemoteCNC.h"
#include "rpc/MethodsRover.h"

namespace romi {

        RemoteCNC::RemoteCNC(std::unique_ptr<rcom::IRPCClient>& rpc_client)
                : RemoteStub(rpc_client)
        {
        }

        bool RemoteCNC::get_range(CNCRange &range)
        {
                r_debug("RemoteCNC::get_range");

                bool success = false;
                nlohmann::json result;

                try {
                        if (execute_with_result(MethodsCNC::kGetRange, result)) {
                                range.init(result);
                                success = true;
                        }
                        
                } catch (nlohmann::json::exception& je) {
                        r_err("RemoteCNC::get_range failed: %s", je.what());
                }

                return success;
        }

        bool RemoteCNC::get_position(v3& position)
        {
                r_debug("RemoteCNC::get_position");

                bool success = false;
                nlohmann::json result;

                try {
                        if (execute_with_result(MethodsCNC::kGetPosition, result)) {
                                position.set(result["x"], result["y"],
                                             result["z"]);
                                success = true;
                        }
                        
                } catch (nlohmann::json::exception& je) {
                        r_err("RemoteCNC::get_position failed: %s", je.what());
                }
                
                return success;
        }

        bool RemoteCNC::moveto(double x, double y, double z, double v)
        {
                r_debug("RemoteCNC::moveto");
                nlohmann::json params;
                
                if (x != UNCHANGED)
                        params[MethodsCNC::kMoveXParam] = x;

                if (y != UNCHANGED)
                    params[MethodsCNC::kMoveYParam] = y;
                
                if (z != UNCHANGED)
                    params[MethodsCNC::kMoveZParam] = z;

                params[MethodsCNC::kSpeedParam] = v;

                return execute_with_params(MethodsCNC::kMoveTo, params);
        }

        // bool RemoteCNC::moveat(double vx, double vy, double vz)
        // {
        //         r_debug("RemoteCNC::moveat");
        //         nlohmann::json params;
        // }

        bool RemoteCNC::spindle(double speed)
        {
                r_debug("RemoteCNC::spindle");

                nlohmann::json params;
                params[MethodsCNC::kSpeedParam] = speed;
                return execute_with_params(MethodsCNC::kSpindle, params);
        }
        
        bool RemoteCNC::travel(Path &path, double relative_speed)
        {
                r_debug("RemoteCNC::travel");

                nlohmann::json parameters;
                nlohmann::json points = nlohmann::json::array();
                for (size_t i = 0; i < path.size(); i++) {
                    points.emplace_back(nlohmann::json{path[i].x(), path[i].y(), path[i].z()} );
                }

                parameters[MethodsCNC::kTravelPathParam] = points;
                parameters[MethodsCNC::kSpeedParam] = relative_speed;

                return execute_with_params(MethodsCNC::kTravel, parameters);
        }

        bool RemoteCNC::helix(double xc, double yc, double alpha, double z, double speed)
        {
                r_debug("RemoteCNC::helix");
                nlohmann::json params {
                        {MethodsCNC::kHelixXcParam, xc},
                        {MethodsCNC::kHelixYcParam, yc},
                        {MethodsCNC::kHelixAlphaParam, alpha},
                        {MethodsCNC::kHelixZParam, z},
                        {MethodsCNC::kSpeedParam, speed}
                };
//
//                        JsonCpp::construct("{\"%s\": %.6f, "
//                                                    "\"%s\": %.6f, "
//                                                    "\"%s\": %.9f, "
//                                                    "\"%s\": %.6f, "
//                                                    "\"%s\": %.3f}",
//                                                    MethodsCNC::kHelixXcParam, xc,
//                                                    MethodsCNC::kHelixYcParam, yc,
//                                                    MethodsCNC::kHelixAlphaParam, alpha,
//                                                    MethodsCNC::kHelixZParam, z,
//                                                    MethodsCNC::kSpeedParam, speed);
                return execute_with_params(MethodsCNC::kHelix, params);
        }

        bool RemoteCNC::homing()
        {
                r_debug("RemoteCNC::homing");
                return execute_simple_request(MethodsCNC::kHoming);
        }

        bool RemoteCNC::synchronize(double timeout_seconds)
        {
                r_debug("RemoteCNC::synchronize");

                nlohmann::json params;
                params[MethodsCNC::kTimeoutParam] = timeout_seconds;
                return execute_with_params(MethodsCNC::kSynchronize, params);
        }

        bool RemoteCNC::pause_activity()
        {
                r_debug("RemoteCNC::pause_activity");
                return execute_simple_request(MethodsActivity::activity_pause);
        }

        bool RemoteCNC::continue_activity()
        {
                r_debug("RemoteCNC::continue_activity");
                return execute_simple_request(MethodsActivity::activity_continue);
        }

        bool RemoteCNC::reset_activity()
        {
                r_debug("RemoteCNC::reset");
                return execute_simple_request(MethodsActivity::activity_reset);
        }

        bool RemoteCNC::power_up()
        {
                r_debug("RemoteCNC::power_up");
                return execute_simple_request(MethodsPowerDevice::power_up);
        }
        
        bool RemoteCNC::power_down()
        {
                r_debug("RemoteCNC::power_down");
                return execute_simple_request(MethodsPowerDevice::power_down);
        }
        
        bool RemoteCNC::stand_by()
        {
                r_debug("RemoteCNC::stand_by");
                return execute_simple_request(MethodsPowerDevice::stand_by);
        }
        
        bool RemoteCNC::wake_up()
        {
                r_debug("RemoteCNC::wake_up");
                return execute_simple_request(MethodsPowerDevice::wake_up);
        }
}
