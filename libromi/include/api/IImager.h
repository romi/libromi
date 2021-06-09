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

#ifndef __ROMI_I_IMAGER_H
#define __ROMI_I_IMAGER_H

#include <session/ISession.h>

namespace romi {
        
        class IImager
        {
        public:
                virtual ~IImager() = default;

                // The max_images and max_images arguments are
                // safeguards. The recording will stop when either the
                // number of images had been reached or the duration
                // has elapsed, whichever comes first. These
                // constraints avoid that a runaway process fills the
                // disk.
                virtual bool start_recording(const std::string &observation_id,
                                             size_t max_images,
                                             double max_duration) = 0;
                
                virtual bool stop_recording() = 0;

                virtual bool is_recording() = 0;
        };
}

#endif // __ROMI_I_IMAGER_H
