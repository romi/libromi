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
#include "hal/CNCAndGimbal.h"

namespace romi {
                
        CNCAndGimbal::CNCAndGimbal(std::unique_ptr<ICNC>& cnc,
                                   std::unique_ptr<IGimbal>& gimbal)
                : cnc_(),
                  gimbal_()
        {
                cnc_ = std::move(cnc);
                gimbal_ = std::move(gimbal);
        }

        bool CNCAndGimbal::get_range(CNCRange &xyz, IRange &angles)
        {
                return (cnc_->get_range(xyz)
                        && gimbal_->get_range(angles));
        }
                
        bool CNCAndGimbal::moveto(double x, double y, double z,
                                  double phi_x, double phi_y, double phi_z,
                                  double relative_speed)
        {
                bool success = (cnc_->moveto(x, y, z, relative_speed)
                                && gimbal_->moveto(phi_x, phi_y, phi_z, relative_speed));
                return success;
        }
        
        bool CNCAndGimbal::get_position(v3& xyz, v3& angles)
        {
                return (cnc_->get_position(xyz)
                        && gimbal_->get_position(angles));
        }

        bool CNCAndGimbal::synchronize(double timeout_seconds)
        {
                return cnc_->synchronize(timeout_seconds)
                        && gimbal_->synchronize(timeout_seconds);
        }

        bool CNCAndGimbal::homing()
        {
                return (cnc_->homing() && gimbal_->homing());
        }
}

