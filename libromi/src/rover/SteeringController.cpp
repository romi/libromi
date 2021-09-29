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
#include "rover/SteeringController.h"
#include <iostream>

namespace romi {

        SteeringController::SteeringController(std::unique_ptr<romiserial::IRomiSerialClient>& romi_serial)
                : _romi_serial(),
                  _mutex(),
                  _continue_condition(),
                  _activity_helper()
        {
                _romi_serial = std::move(romi_serial);
        }

        int SteeringController::send_command(const char *command)
        {
                int r = -1;
                JsonCpp response;
                auto clock = rpp::ClockAccessor::GetInstance();

                /* The number of loops is a bit random but it avoids
                 * an infinite loop. The loop will take at the most 10
                 * seconds to complete. This gives the firmware 10
                 * seconds to free a slot in the command buffer. */
                for (int i = 0; i < 1000; i++) {

                        //r_debug("SteeringController::send_command: %s", command);
                        
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
                                        r_err("SteeringController::execute: "
                                              "error: %s", response.str(1));
                                        break;
                                }
                                
                        } else {
                                std::string s;
                                response.tostring(s, k_json_compact);
                                r_debug("SteeringController::send_command: "
                                        "invalid response: %s", s.c_str());
                        }
                }
                        
                return r;
        }
        
        bool SteeringController::homing()
        {
                r_info("SteeringController: homing");
                return (send_command("H") == 0 && synchronize(120.0));
        }
        
        bool SteeringController::move(int16_t dt, int16_t steps_left, int16_t steps_right)
        {
                char buffer[64];
                rprintf(buffer, 64, "M[%d,%d,%d]", dt, steps_left, steps_right);
                return (send_command(buffer) == 0);
        }

        bool SteeringController::moveat(int16_t speed_left, int16_t speed_right)
        {
                char buffer[64];
                rprintf(buffer, 64, "V[%hd,%hd]", speed_left, speed_right);
                return (send_command(buffer) == 0);
        }
        
        bool SteeringController::moveto(int16_t dt, int16_t pos_left, int16_t pos_right)
        {
                char buffer[64];
                rprintf(buffer, 64, "m[%d,%d,%d]", dt, pos_left, pos_right);
                return (send_command(buffer) == 0);                
        }

        int SteeringController::is_idle()
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
                                        r_err("SteeringController::is_idle: "
                                              "Firmware in error state");
                                        idle = -1;
                                }
                                
                        } else {
                                r_err("SteeringController::is_idle: error: "
                                      "invalid array length");
                        }
                        
                } else {
                        r_err("SteeringController::is_idle: error: %s", s.str(1));
                }
                
                return idle;
        }
        
        bool SteeringController::get_position(int32_t *pos)
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
                                r_debug("pos[0]=%d, pos[1]=%d",pos[0],pos[1]);
                        } else {
                                r_err("SteeringController::get_position: error: "
                                      "invalid array length");
                                r = -1;
                        }
                } else {
                        r_err("SteeringController::get_position: error: %s", s.str(1));
                }
                
                return success;
        }

        bool SteeringController::synchronize(double timeout)
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
                                r_warn("SteeringController::synchronize: time out");
                                break;
                        }
                }
                return success;
        }

        bool SteeringController::response_ok(JsonCpp& response)
        {
                return  (response.isarray()
                         && response.get(0).isnumber()
                         && response.num(0) == 0);
        }
        
        bool SteeringController::send_command_without_interruption(const char *command)
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
        
        bool SteeringController::pause_activity()
        {
                r_info("SteeringController: pausing");
                bool retval = send_command_without_interruption("p");
                _activity_helper.pause_activity();
                return retval;
        }
        
        bool SteeringController::continue_activity()
        {
                r_info("SteeringController: continuing");
                _activity_helper.continue_activity();
                return send_command_without_interruption("c");
        }
        
        bool SteeringController::reset_activity()
        {
                r_info("SteeringController: resetting");
                _activity_helper.reset_activity();
                return send_command_without_interruption("r");
        }

        bool SteeringController::enable()
        {
                r_info("SteeringController: enabling");
                return send_command_without_interruption("E[1]");
        }
        
        bool SteeringController::disable()
        {
                r_info("SteeringController: disabling");
                return send_command_without_interruption("E[0]");
        }

        bool SteeringController::stop()         //???? utiliser la fonction de BrushMotorDriver en la passant en static pour eviter les doublons?
        {
            JsonCpp response;
            const char *command = "V[0,0,0]"; //Avec X ca ne marche pas
            _romi_serial->send(command, response);
            return check_response(command, response);
        }

        bool SteeringController::check_response(const char *command, JsonCpp& response)
        {
            bool success = (response.num(romiserial::kStatusCode) == 0);
            if (!success) {
                const char *message = "No message";
                if (response.length() > 1)
                    message = response.str(romiserial::kErrorMessage);
                r_warn("StepperMotorDriver: command %s returned error: %s",
                       command, message);
            }
            return success;
        }
}
