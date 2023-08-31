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
#ifndef __ROMI_METHODS_CNC_H
#define __ROMI_METHODS_CNC_H

namespace romi {
        
        class MethodsCNC
        {
        public:
                
                static constexpr const char *kHoming = "cnc-homing";
                static constexpr const char *kSetHoming = "cnc-set-homing";
                static constexpr const char *kMoveTo = "cnc-moveto";
                //static constexpr const char *moveat = "cnc-moveat";
                static constexpr const char *kSpindle = "cnc-spindle";
                static constexpr const char *kTravel = "cnc-travel";
                static constexpr const char *kGetRange = "cnc-get-range";
                static constexpr const char *kHelix = "cnc-helix";
                static constexpr const char *kGetPosition = "cnc-get-position";
                static constexpr const char *kSynchronize = "cnc-synchronize";
                static constexpr const char *kSetRelay = "set-relay";

                static constexpr const char *kSpeedParam = "speed";
                static constexpr const char *kMoveXParam = "x";
                static constexpr const char *kMoveYParam = "y";
                static constexpr const char *kMoveZParam = "z";
                static constexpr const char *kTravelPathParam = "path";
                static constexpr const char *kHelixXcParam = "xc";
                static constexpr const char *kHelixYcParam = "yc";
                static constexpr const char *kHelixAlphaParam = "alpha";
                static constexpr const char *kHelixZParam = "z";
                static constexpr const char *kTimeoutParam = "timeout";
                static constexpr const char *kIndex = "index";
                static constexpr const char *kValue = "value";
        };
}

#endif // __ROMI_METHODS_CNC_H
