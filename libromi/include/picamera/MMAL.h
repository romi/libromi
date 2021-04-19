/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

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
#ifndef _LIBROMI_MMAL_H_
#define _LIBROMI_MMAL_H_

#include "PiCameraIncludes.h"

namespace romi::arm {

        class MMAL {
        public:
                MMAL() {}
                virtual ~MMAL() = default;

                void assert_status(const char *caller_name, MMAL_STATUS_T status);
        };
}

#endif // _LIBROMI_MMAL_H_
