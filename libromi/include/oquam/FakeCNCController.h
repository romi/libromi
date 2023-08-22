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
#ifndef _OQUAM_FAKE_CNC_CONTROLLER_HPP_
#define _OQUAM_FAKE_CNC_CONTROLLER_HPP_

#include "oquam/ICNCController.h"

namespace romi {

        class FakeCNCController : public ICNCController
        {
        public:
                static constexpr const char *ClassName = "fake-cnc-controller";
                
        protected:
                int32_t _pos[3];
                
        public:
                FakeCNCController() {
                        homing();
                }
                        
                virtual ~FakeCNCController() override = default;

                bool set_homing_axes(AxisIndex, AxisIndex, AxisIndex) override {
                        return true;
                }
                
                bool set_homing_mode(HomingMode) override {
                        return true;
                }
                
                bool set_homing_speeds(int16_t, int16_t, int16_t) override {
                        return true;
                }

                bool get_position(int32_t *pos) override {
                        for (int i = 0; i < 3; i++)
                                pos[i] = _pos[i];
                        return true;
                }

                bool homing() override {
                        for (int i = 0; i < 3; i++)
                                _pos[i] = 0;
                        return true;
                }
                
                bool spindle(double speed) override
                {
                        (void) speed;
                        return true;
                }
                
                bool synchronize(__attribute__((unused))double timeout) override{
                        return true;
                }
                
                bool move(int16_t millis, int16_t steps_x,
                          int16_t steps_y, int16_t steps_z) override {
                        (void) millis;
                        _pos[0] += steps_x;
                        _pos[1] += steps_y;
                        _pos[2] += steps_z;
                        return true;
                }
                bool moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z) override
                {   (void)speed_x;
                    (void)speed_y;
                    (void)speed_z;
                    return true;
                };
                
                bool moveto(int16_t dt, int16_t x, int16_t y, int16_t z) override {
                        (void) dt;
                        _pos[0] = x;
                        _pos[1] = y;
                        _pos[2] = z;
                        return true;
                }
                
                bool pause_activity() override {
                        return true;
                }
                
                bool continue_activity() override {
                        return true;
                }
                
                bool reset_activity() override {
                        return homing();
                }

                bool enable() override {
                        return true;
                }
                
                bool disable() override {
                        return true;
                }

                bool stop() override {
                    return true;
                }
        };
}

#endif // _OQUAM_FAKE_CNC_CONTROLLER_HPP_
