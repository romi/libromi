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

#ifndef __ROMI_FAKE_IMAGER_H
#define __ROMI_FAKE_IMAGER_H

#include "api/IImager.h"

namespace romi {
        
        class FakeImager : public IImager
        {
        public:
                FakeImager() = default;
                ~FakeImager() override = default;
                
                bool start_recording(const std::string& observation_id,
                                     size_t max_images,
                                     double max_duration) override {
                        (void) observation_id;
                        (void) max_images;
                        (void) max_duration;
                        r_warn("FakeImager::start_recording: No doing anything!");
                        return true;
                }
                
                bool stop_recording() override {
                        r_warn("FakeImager::stop_recording: No doing anything!");
                        return true;
                }
        };
}

#endif // __ROMI_FAKE_IMAGER_H
