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
#ifndef __ROMI_FAKE_CNC_H
#define __ROMI_FAKE_CNC_H

#include <rcom.h>
#include <romi/IConfiguration.h>
#include <romi/ICNC.h>

namespace romi {
        
        class FakeCNC : public ICNC
        {
        protected:
                CNCRange _range;
                
        public:
                FakeCNC(IConfiguration &config) {
                        _range.init(config.get("cnc").get("range"));
                }
                
                virtual ~FakeCNC() override = default;
                
                void get_range(CNCRange &range) override {
                        range = _range;
                }
                
                void moveto(double x, double y, double z,
                            double relative_speed = 0.1) override {}
                
                void spindle(double speed) override {}
                void travel(Path &path, double relative_speed = 0.1) override {}
                void homing() {}
                void stop_execution() override {}
                void continue_execution() override {}
                void reset() override {}
        };
}

#endif // __ROMI_FAKE_CNC_H
