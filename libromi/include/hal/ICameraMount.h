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
#ifndef __ROMI_I_CAMERAMOUNT_H
#define __ROMI_I_CAMERAMOUNT_H

#include "api/IActivity.h"
#include "api/IPowerDevice.h"
#include "api/IRange.h"
#include "api/CNCRange.h"

namespace romi {
        
        class ICameraMount : public IActivity, public IPowerDevice
        {
        public:

                virtual ~ICameraMount() = default;

                virtual bool homing() = 0; 
                virtual bool get_position(v3& xyz, v3& angles) = 0;
                virtual bool get_range(CNCRange &xyz, IRange &angles) = 0;
                virtual bool moveto(double x, double y, double z,
                                    double phi_x, double phi_y, double phi_z,
                                    double relative_speed) = 0;
                virtual bool synchronize(double timeout_seconds) = 0; 
        };
}

#endif // __ROMI_CAMERAMOUNT_H
