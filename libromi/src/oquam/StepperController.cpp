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
#include <stdexcept>
#include <JsonCpp.h> 
#include <RomiSerialErrors.h>
#include <ClockAccessor.h>
#include "oquam/StepperController.h" 

namespace romi {

        StepperController::StepperController(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial)
                : _romi_serial(),
                  _mutex(),
                  _continue_condition(),
                  _activity_helper()
        {
                _romi_serial = std::move(romi_serial);
        }

        int StepperController::send_command(const char *command)
        {
                int r = -1;
                JsonCpp response;
                auto clock = rpp::ClockAccessor::GetInstance();

                /* The number of loops is a bit random but it avoids
                 * an infinite loop. The loop will take at the most 10
                 * seconds to complete. This gives the firmware 10
                 * seconds to free a slot in the command buffer. */
                for (int i = 0; i < 1000; i++) {

                        //r_debug("StepperController::send_command: %s", command);
                        
                        _activity_helper.check_pause_or_cancel_execution();
                        
                        _romi_serial->send(command, response);

                        if (response.isarray()
                            && response.get(0).isnumber()) {
                                
                                r = (int) response.num(0);
                                
                                        
                                if (r == 0) {
                                        break;
                                
                                } else if (r == 1) {
                                        // Error code 1 indicates that the
                                        // command buffer in the firmware is
                                        // full. Wait a bit and try again.
                                        clock->sleep(0.2);
                                                
                                } else {
                                        r_err("StepperController::execute: "
                                              "error: %s", response.str(1));
                                        break;
                                }
                                
                        } else {
                                std::string s;
                                response.tostring(s, k_json_compact);
                                r_debug("StepperController::send_command: "
                                        "invalid response: %s", s.c_str());
                        }
                }
                        
                return r;
        }
        
        bool StepperController::homing()
        {
                r_info("StepperController: homing");
                return (send_command("H") == 0 && synchronize(120.0));
        }

        bool StepperController::spindle(double speed)
        {
                const char* command = (speed == 0.0)? "S[0]" : "S[1]";
                return (send_command(command) == 0);
        }
        
        bool StepperController::move(int16_t dt, int16_t dx, int16_t dy, int16_t dz)
        {
                char buffer[64];
                rprintf(buffer, 64, "M[%d,%d,%d,%d]", dt, dx, dy, dz);
                return (send_command(buffer) == 0);
        }

        int StepperController::is_idle()
        {
                int idle = -1;
                int state = '?';
                
                JsonCpp s;
                _romi_serial->send("I", s);

                int r = (int) s.num(0);
                if (r == 0) {
                        if (s.length() == 3) {
                                // This is the answer to "is idle?".
                                idle = (int) s.num(1);

                                const char *t = s.str(2);
                                state = t[0];

                                // TODO
                                // If the driver is in an error state,
                                // report it.
                                if (state == 'e') {
                                        // FIXME: what shall we do?
                                        r_err("StepperController::is_idle: "
                                              "Firmware in error state");
                                        idle = -1;
                                }
                                
                        } else {
                                r_err("StepperController::is_idle: error: "
                                      "invalid array length");
                        }
                        
                } else {
                        r_err("StepperController::is_idle: error: %s", s.str(1));
                }
                
                return idle;
        }
        
        bool StepperController::get_position(int32_t *pos)
        {
                bool success = false;
                
                JsonCpp s;
                _romi_serial->send("P", s);

                int r = (int) s.num(0);
                if (r == 0) {
                        if (s.length() == 4) {
                                pos[0] = (int32_t) s.num(1);
                                pos[1] = (int32_t) s.num(2);
                                pos[2] = (int32_t) s.num(3);
                                success = true;
                        } else {
                                r_err("StepperController::get_position: error: "
                                      "invalid array length");
                                r = -1;
                        }
                } else {
                        r_err("StepperController::get_position: error: %s", s.str(1));
                }
                
                return success;
        }

        bool StepperController::synchronize(double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                bool success = false;
                double start_time = clock->time();
                
                while (true) {

                        int idle = is_idle();
                        
                        if (idle == 1) {
                                success = true;
                                break;
                        } else if (idle == -1) {
                                break;
                        } else {
                                clock->sleep(0.2);
                        }

                        double now = clock->time();
                        if (timeout >= 0.0 && (now - start_time) >= timeout) {
                                r_warn("StepperController::synchronize: time out");
                                break;
                        }
                }
                return success;
        }

        bool StepperController::response_ok(JsonCpp& response)
        {
                return  (response.isarray()
                         && response.get(0).isnumber()
                         && response.num(0) == 0);
        }
        
        bool StepperController::send_command_without_interruption(const char *command)
        {
                bool success = false;
                JsonCpp response;
                for (int i = 0; i < 10; i++) {
                        _romi_serial->send(command, response);
                        if (response_ok(response)) {
                                success = true;
                                break;
                        }
                }
                return success;
        }
        
        bool StepperController::pause_activity()
        {
                r_info("StepperController: pausing");
                bool retval = send_command_without_interruption("p");
                _activity_helper.pause_activity();
                return retval;
        }
        
        bool StepperController::continue_activity()
        {
                r_info("StepperController: continuing");
                _activity_helper.continue_activity();
                return send_command_without_interruption("c");
        }
        
        bool StepperController::reset_activity()
        {
                r_info("StepperController: resetting");
                _activity_helper.reset_activity();
                return send_command_without_interruption("r");
        }

        bool StepperController::enable()
        {
                r_info("StepperController: enabling");
                return send_command_without_interruption("E[1]");
        }
        
        bool StepperController::disable()
        {
                r_info("StepperController: disabling");
                return send_command_without_interruption("E[0]");
        }
        
        bool StepperController::configure_homing(AxisIndex axis1, AxisIndex axis2,
                                                 AxisIndex axis3)
        {
                char command[64];
                rprintf(command, 64, "h[%d,%d,%d]", axis1, axis2, axis3);
                r_info("StepperController: setting homing to [%d,%d,%d]", axis1, axis2, axis3);
                return send_command_without_interruption(command);
        }
}
