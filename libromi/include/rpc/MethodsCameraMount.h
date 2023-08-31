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
#ifndef __ROMI_METHODSCAMERAMOUNT_H
#define __ROMI_METHODSCAMERAMOUNT_H

namespace romi {
        
        class MethodsCameraMount
        {
        public:
                
                static constexpr const char *kMoveto = "camera-mount:moveto";
                static constexpr const char *kGetRange = "camera-mount:get-range";
                static constexpr const char *kGetPosition = "camera-mount:get-position";
                static constexpr const char *kHoming = "camera-mount:homing";
                static constexpr const char *kSetRelay = "camera-mount:set-relay";

                static constexpr const char *kSpeedParam = "speed";
                static constexpr const char *kXParam = "x";
                static constexpr const char *kYParam = "y";
                static constexpr const char *kZParam = "z";
                static constexpr const char *kAngleXParam = "ax";
                static constexpr const char *kAngleYParam = "ay";
                static constexpr const char *kAngleZParam = "az";
                static constexpr const char *kPositionX = "x";
                static constexpr const char *kPositionY = "y";
                static constexpr const char *kPositionZ = "z";
                static constexpr const char *kPositionAngleX = "ax";
                static constexpr const char *kPositionAngleY = "ay";
                static constexpr const char *kPositionAngleZ = "az";
                static constexpr const char *kXYZRange = "xyz-range";
                static constexpr const char *kAnglesRange = "angles-range";
                static constexpr const char *kIndex = "index";
                static constexpr const char *kValue = "value";
        };
}

#endif // __ROMI_METHODSCAMERAMOUNT_H
