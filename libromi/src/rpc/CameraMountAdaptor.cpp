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
#include "rpc/CameraMountAdaptor.h"
#include "rpc/MethodsCameraMount.h"
#include "rpc/MethodsPowerDevice.h"
#include "rpc/MethodsActivity.h"
#include "api/CNCRange.h"

namespace romi {
        
        CameraMountAdaptor::CameraMountAdaptor(ICameraMount& mount)
                : mount_(mount)
        {
        }

        void CameraMountAdaptor::execute(const std::string& id,
                                         const std::string& method,
                                         nlohmann::json &params,
                                         rcom::MemBuffer& result,
                                         rcom::RPCError &error)
        {
                (void) id;
                (void) method;
                (void) params;
                (void) result;
                error.code = rcom::RPCError::kMethodNotFound;
                error.message = "Unknown method";
        }
        
        void CameraMountAdaptor::execute(const std::string& id,
                                         const std::string& method,
                                         nlohmann::json& params,
                                         nlohmann::json& result,
                                         rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute");
                
                (void) id;
                error.code = 0;
                
                try {

                        if (method == MethodsCameraMount::kMoveto) {
                                execute_moveto(params, error);
                        
                        } else if (method == MethodsCameraMount::kGetPosition) {
                                execute_get_position(result, error);
                        
                        } else if (method == MethodsCameraMount::kGetRange) {
                                execute_get_range(result, error);
                        
                        } else if (method == MethodsCameraMount::kHoming) {
                                execute_homing(error);
                        
                        } else if (method == MethodsCameraMount::kSetRelay) {
                                execute_set_relay(params);
                        
                        } else if (method == MethodsActivity::activity_pause) {
                                execute_pause(error);
                                
                        } else if (method == MethodsActivity::activity_continue) {
                                execute_continue(error);
                                
                        } else if (method == MethodsActivity::activity_reset) {
                                execute_reset(error);
                                
                        } else if (method == MethodsPowerDevice::power_up) {
                                execute_power_up(error);
                                
                        } else if (method == MethodsPowerDevice::power_down) {
                                execute_power_down(error);
                                
                        } else if (method == MethodsPowerDevice::stand_by) {
                                execute_stand_by(error);
                                
                        } else if (method == MethodsPowerDevice::wake_up) {
                                execute_wake_up(error);
                                
                        } else {
                                error.code = rcom::RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception &e) {
                        error.code = rcom::RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void CameraMountAdaptor::execute_moveto(nlohmann::json& params,
                                                rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute_moveto");
                double x = params[MethodsCameraMount::kXParam];
                double y = params[MethodsCameraMount::kYParam];
                double z = params[MethodsCameraMount::kZParam];
                double ax = params[MethodsCameraMount::kAngleXParam];
                double ay = params[MethodsCameraMount::kAngleYParam];
                double az = params[MethodsCameraMount::kAngleZParam];
                double v = params[MethodsCameraMount::kSpeedParam];
                if (!mount_.moveto(x, y, z, ax, ay, az, v)) {
                        r_err("CameraMountAdaptor::execute_moveto failed");
                        error.code = 1;
                        error.message = "Moveto failed";
                }
        }

        void CameraMountAdaptor::execute_get_position(nlohmann::json& result,
                                                      rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::get_position");
                v3 xyz;
                v3 angles;
                if (mount_.get_position(xyz, angles)) {
                        result[MethodsCameraMount::kPositionX] = xyz.x();
                        result[MethodsCameraMount::kPositionY] = xyz.y();
                        result[MethodsCameraMount::kPositionZ] = xyz.z();
                        result[MethodsCameraMount::kPositionAngleX] = angles.x();
                        result[MethodsCameraMount::kPositionAngleY] = angles.y();
                        result[MethodsCameraMount::kPositionAngleZ] = angles.z();
                } else {
                        r_err("CameraMountAdaptor::execute_get_position failed");
                        error.code = 1;
                        error.message = "Get position failed";
                }
        }

        void CameraMountAdaptor::execute_get_range(nlohmann::json& result,
                                                   rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::get_range");
                CNCRange xyz_range;
                Range angle_range;
                if (mount_.get_range(xyz_range, angle_range)) {
                        result = {
                                {MethodsCameraMount::kXYZRange, xyz_range.to_json()},
                                {MethodsCameraMount::kAnglesRange, angle_range.to_json()}
                        };
                } else {
                        r_err("CameraMountAdaptor::execute_get_range failed");
                        error.code = 1;
                        error.message = "Get range failed";
                }
        }
        
        void CameraMountAdaptor::execute_homing(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute_homing");
                
                if (!mount_.homing()) {
                        error.code = 1;
                        error.message = "homing failed";
                }
        }

        void CameraMountAdaptor::execute_pause(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute_pause");
                if (!mount_.pause_activity()) {
                        r_err("CameraMountAdaptor::execute_pause failed");
                        error.code = 1;
                        error.message = "stop failed";
                }
        }

        void CameraMountAdaptor::execute_continue(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute_continue");
                if (!mount_.continue_activity()) {
                        r_err("CameraMountAdaptor::execute_continue failed");
                        error.code = 1;
                        error.message = "continue failed";
                }
        }

        void CameraMountAdaptor::execute_reset(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::execute_reset");
                if (!mount_.reset_activity()) {
                        r_err("CameraMountAdaptor::execute_reset failed");
                        error.code = 1;
                        error.message = "reset failed";
                }
        }

        void CameraMountAdaptor::execute_power_up(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::power_up");
                if (!mount_.power_up()) {
                        r_err("CameraMountAdaptor::execute_power_up failed");
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void CameraMountAdaptor::execute_power_down(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::power_down");
                if (!mount_.power_down()) {
                        r_err("CameraMountAdaptor::execute_power_down failed");
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void CameraMountAdaptor::execute_stand_by(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::stand_by");
                if (!mount_.stand_by()) {
                        r_err("CameraMountAdaptor::execute_stand_by failed");
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void CameraMountAdaptor::execute_wake_up(rcom::RPCError &error)
        {
                r_debug("CameraMountAdaptor::wake_up");
                if (!mount_.wake_up()) {
                        r_err("CameraMountAdaptor::execute_wake_up failed");
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }

        void CameraMountAdaptor::execute_set_relay(nlohmann::json& params)
        {
                r_debug("CameraMountAdaptor::set_relay");
                uint8_t index = params[MethodsCameraMount::kIndex];
                bool value = params[MethodsCameraMount::kValue];
                mount_.set_relay(index, value);
        }
}
