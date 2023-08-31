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
#include <iostream>
#include "util/Logger.h"
#include "rpc/CNCAdaptor.h"
#include "rpc/MethodsRover.h"

namespace romi {

        CNCAdaptor::CNCAdaptor(ICNC &cnc)
                : cnc_(cnc)
        {
        }

        void CNCAdaptor::execute(const std::string& id,
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

        void CNCAdaptor::execute(const std::string& id,
                                 const std::string& method,
                                 nlohmann::json& params,
                                 nlohmann::json& result,
                                 rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute");

                (void) id;
                error.code = 0;
                
                try {

                        if (method.empty()) {
                                error.code = rcom::RPCError::kMethodNotFound;
                                error.message = "No method specified";
                                
                        } else if (method == MethodsCNC::kHoming) {
                                execute_homing(error);
                                 
                        } else if (method == MethodsCNC::kMoveTo) {
                                execute_moveto(params, error);
                                
                        } else if (method == MethodsCNC::kSpindle) {
                                execute_spindle(params, error);
                                
                        } else if (method == MethodsCNC::kTravel) {
                                execute_travel(params, error);
                                
                        } else if (method == MethodsCNC::kHelix) {
                                execute_helix(params, error);
                                
                        } else if (method == MethodsCNC::kGetRange) {
                                execute_get_range(result, error);
                                
                        } else if (method == MethodsCNC::kGetPosition) {
                                execute_get_position(result, error);
                                
                        } else if (method == MethodsCNC::kSynchronize) {
                                execute_synchronize(params, error);
                                
                        } else if (method == MethodsCNC::kSetRelay) {
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
                                r_err("CNCAdaptor::execute: method not found: %s",
                                      method.c_str());
                                error.code = rcom::RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }

                } catch (std::exception &e) {
                        error.code = rcom::RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void CNCAdaptor::execute_get_range(nlohmann::json& result, rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_get_range");
                CNCRange range;
                if (cnc_.get_range(range)) {
                    result = nlohmann::json::array({
                                    nlohmann::json::array(
                                            {range.xmin(), range.xmax()}),
                                            nlohmann::json::array(
                                                    {range.ymin(), range.ymax()}),
                                            nlohmann::json::array(
                                                    {range.zmin(), range.zmax()})
                                            });
                } else {
                        r_err("CNCAdaptor::execute_get_range failed");
                        error.code = 1;
                        error.message = "get_range failed";
                }
        }

        void CNCAdaptor::execute_get_position(nlohmann::json& result,
                                              rcom::RPCError &error)
        {
            r_debug("CNCAdaptor::execute_get_position");
            v3 position;
            if (cnc_.get_position(position)) {
//                result = JsonCpp::construct("{\"x\":%f,\"y\":%f,\"z\":%f}",
//                                            position.x(), position.y(),
//                                            position.z());

                result = {{MethodsCNC::kMoveXParam, position.x()},
                          {MethodsCNC::kMoveYParam, position.y()},
                          {MethodsCNC::kMoveZParam, position.z()}};
            } else {
                r_err("CNCAdaptor::execute_get_position failed");
                error.code = 1;
                error.message = "get_position failed";
            }
        }

        void CNCAdaptor::execute_synchronize(nlohmann::json& params,
                                             rcom::RPCError &error)
        {
            r_debug("CNCAdaptor::execute_synchronize");
            
            double timeout = params[MethodsCNC::kTimeoutParam];
            
            if (!cnc_.synchronize(timeout)) {
                r_err("CNCAdaptor::execute_synchronize failed");
                error.code = 1;
                error.message = "get_position failed";
            }
        }

        void CNCAdaptor::execute_moveto(nlohmann::json& params, rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_moveto");
                {
                        r_debug("CNCAdaptor::execute_moveto: %s", params.dump().c_str());
                }

                if (!params.contains(MethodsCNC::kMoveXParam)
                    && !params.contains(MethodsCNC::kMoveYParam)
                    && !params.contains(MethodsCNC::kMoveZParam)) {
                        r_err("CNCAdaptor::execute_moveto failed: missing parameters");
                        error.code = rcom::RPCError::kInvalidParams;
                        error.message = "missing x, y, or z parameters";
                        
                } else {
                        double x = params.value(MethodsCNC::kMoveXParam, ICNC::UNCHANGED);
                        double y = params.value(MethodsCNC::kMoveYParam, ICNC::UNCHANGED);
                        double z = params.value(MethodsCNC::kMoveZParam, ICNC::UNCHANGED);
                        double v = params.value(MethodsCNC::kSpeedParam, 0.2);
                        
                        r_debug("CNCAdaptor::execute_moveto: %f, %f, %f", x, y, z);
                                
                        if (!cnc_.moveto(x, y, z, v)) {
                                error.code = 1;
                                error.message = "moveto failed";
                        }
                }
        }

        void CNCAdaptor::execute_spindle(nlohmann::json& params, rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_spindle");
                
                try {
                        double speed = params[MethodsCNC::kSpeedParam];

                        if (!cnc_.spindle(speed)) {
                                error.code = 1;
                                error.message = "spindle failed";
                        }

                } catch (nlohmann::json::exception & je) {
                        r_err("CNCAdaptor::execute_spindle failed: %s", je.what());
                        error.code = rcom::RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }

        void CNCAdaptor::execute_travel(nlohmann::json& params, rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_travel");
                
                try {
                        Path path;
                        double speed = params.value(MethodsCNC::kSpeedParam, 0.1);
                        nlohmann::json p = params.at(MethodsCNC::kTravelPathParam);
                        for (auto & i : p) {
                                path.emplace_back(i.at(0), i.at(1), i.at(2));
                        }

                        if (!cnc_.travel(path, speed)) {
                                error.code = 1;
                                error.message = "travel failed";
                        }

                } catch (nlohmann::json::exception& je) {
                        r_err("CNCAdaptor::execute_travel failed: %s", je.what());
                        error.code = rcom::RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }
        
        void CNCAdaptor::execute_helix(nlohmann::json& params, rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_helix");
                
                try {
                        double xc = params[MethodsCNC::kHelixXcParam];
                        double yc = params[MethodsCNC::kHelixYcParam];
                        double alpha = params[MethodsCNC::kHelixAlphaParam];
                        double z = params[MethodsCNC::kHelixZParam];
                        double speed = params.value(MethodsCNC::kSpeedParam, 0.1);
                        
                        if (!cnc_.helix(xc, yc, alpha, z, speed)) {
                                error.code = 1;
                                error.message = "helix failed";
                        }

                } catch (nlohmann::json::exception& je) {
                        r_err("CNCAdaptor::execute_helix failed: %s", je.what());
                        error.code = rcom::RPCError::kInvalidParams;
                        error.message = je.what();
                }
        }
        
        void CNCAdaptor::execute_homing(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_homing");
                if (!cnc_.homing()) {
                        error.code = 1;
                        error.message = "homing failed";
                }
        }

        void CNCAdaptor::execute_set_relay(nlohmann::json& params)
        {
                r_debug("CNCAdaptor::set_relay");
                uint8_t index = params[MethodsCNC::kIndex];
                bool value = params[MethodsCNC::kValue];
                cnc_.set_relay(index, value);
        }

        void CNCAdaptor::execute_pause(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_pause");
                if (!cnc_.pause_activity()) {
                        error.code = 1;
                        error.message = "stop failed";
                }
        }

        void CNCAdaptor::execute_continue(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_continue");
                if (!cnc_.continue_activity()) {
                        error.code = 1;
                        error.message = "continue failed";
                }
        }

        void CNCAdaptor::execute_reset(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::execute_reset");
                if (!cnc_.reset_activity()) {
                        error.code = 1;
                        error.message = "reset failed";
                }
        }

        void CNCAdaptor::execute_power_up(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::power_up");
                if (!cnc_.power_up()) {
                        error.code = 1;
                        error.message = "power up failed";
                }
        }
        
        void CNCAdaptor::execute_power_down(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::power_down");
                if (!cnc_.power_down()) {
                        error.code = 1;
                        error.message = "power down failed";
                }
        }
        
        void CNCAdaptor::execute_stand_by(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::stand_by");
                if (!cnc_.stand_by()) {
                        error.code = 1;
                        error.message = "stand_by failed";
                }
        }
        
        void CNCAdaptor::execute_wake_up(rcom::RPCError &error)
        {
                r_debug("CNCAdaptor::wake_up");
                if (!cnc_.wake_up()) {
                        error.code = 1;
                        error.message = "wake_up failed";
                }
        }
}

