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
#ifndef __ROMI_NAVIGATION_SETTINGS_H
#define __ROMI_NAVIGATION_SETTINGS_H

#include <JsonCpp.h>

namespace romi {
        
        class NavigationSettings
        {
        public:

                static constexpr const char *kEncoderStepsKey = "encoder-steps"; 
                static constexpr const char *kWheelDiameterKey = "wheel-diameter"; 
                static constexpr const char *kMaximumSpeedKey = "maximum-speed"; 
                static constexpr const char *kWheelBaseKey = "wheel-base"; 
                static constexpr const char *kMaximumAccelerationKey = "maximum-acceleration"; 
                
                double encoder_steps;
                double wheel_diameter;
                double maximum_speed;
                double wheel_base;
                double wheel_circumference;
                double max_revolutions_per_sec;
                double maximum_acceleration;

                NavigationSettings(JsonCpp &config);
                virtual ~NavigationSettings() = default;
        };
}

#endif // __ROMI_NAVIGATION_SETTINGS_H
