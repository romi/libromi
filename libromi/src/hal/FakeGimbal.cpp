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
#include "hal/FakeGimbal.h"
#include "util/Logger.h"

namespace romi {
        
        FakeGimbal::FakeGimbal()
                : x_(0.0),
                  y_(0.0),
                  z_(0.0)
        {
        }

        bool FakeGimbal::moveto(double x, double y, double z, double relative_speed)
        {
                (void) relative_speed;
                r_debug("FakeGimbal::moveto: x=%0.2f, y=%0.2f, z=%0.2f", x, y, z);
                x_ = x;
                y_ = y;
                z_ = z;
                return true;
        }
                        
        bool FakeGimbal::get_position(v3& position)
        {
                position.set(x_, y_, z_);
                return true;
        }

        bool FakeGimbal::synchronize(double)
        {
                return true;
        }
        
        bool FakeGimbal::get_range(IRange& range)
        {
                v3 min(-360.0, 0.0, 0.0);
                v3 max(360.0, 0.0, 0.0);
                range.init(min, max);
                return true;
        }
        
        bool FakeGimbal::homing()
        {
                r_debug("FakeGimbal::homing");
                x_ = 0.0;
                y_ = 0.0;
                z_ = 0.0;
                return true;
        }

        bool FakeGimbal::pause_activity()
        {
                return true;
        }

        bool FakeGimbal::continue_activity()
        {
                return true;
        }

        bool FakeGimbal::reset_activity()
        {
                return true;
        }

        bool FakeGimbal::power_up()
        {
                r_debug("FakeGimbal::power_up");
                return true;
        }

        bool FakeGimbal::power_down()
        {
                r_debug("FakeGimbal::power_down");
                return true;
        }

        bool FakeGimbal::stand_by()
        {
                return true;
        }

        bool FakeGimbal::wake_up()
        {
                return true;
        }
}
