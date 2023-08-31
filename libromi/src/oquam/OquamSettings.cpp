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
#include "oquam/OquamSettings.h"

namespace romi {
        
        OquamSettings::OquamSettings(CNCRange& range,
                                     const double *vmax,
                                     const double *amax,
                                     const double *scale_meters_to_steps, 
                                     double path_max_deviation,
                                     double path_slice_duration,
                                     double path_max_slice_duration,
                                     const AxisIndex *homing_axes,
                                     HomingMode homing_mode)
                : range_(range),
                  vmax_(vmax),
                  amax_(amax),
                  path_max_deviation_(path_max_deviation),
                  path_slice_duration_(path_slice_duration),
                  path_max_slice_duration_(path_max_slice_duration),
                  homing_mode_(homing_mode)
        {
                // 32 seconds = 32000 ms < 2^16/2, which is the
                // maximum value in the int16_t used to send block
                // moves.
                if (path_max_slice_duration_ > 32.0)
                        path_max_slice_duration_ = 32.0;
                
                vcopy(scale_meters_to_steps_, scale_meters_to_steps);
                for (int i = 0; i < 3; i++) {
                        homing_axes_[i] = homing_axes[i];
                }
        }
}
