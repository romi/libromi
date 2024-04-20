/*StillCamera
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
#include <stdexcept>

#include "picamera/StillCamera.h"
#include "util/Logger.h"

namespace romi {
        
        StillCamera::StillCamera(PiCameraSettings& settings)
                : //camera_(settings),
                  BaseCamera(settings),
                  encoder_(),
                  encoder_connection_(camera_.get_still_port(),
                                      encoder_.get_input_port())
        {
        }

        StillCamera::~StillCamera()
        {
                r_info("StillCamera: deleted");
        }
        
        bool StillCamera::grab(romi::Image &image)
        {
                (void) image;
                r_err("StillCamera::grab not implemented, yet");
                return false;
        }

        rcom::MemBuffer& StillCamera::grab_jpeg()
        {
                encoder_.prepare_capture();
                camera_.init_shutter_speed();
                camera_.trigger_capture();
                encoder_.finish_capture();
                return encoder_.get_buffer();
        }

        bool StillCamera::set_jpeg_quality(uint32_t value)
        {
                return encoder_.set_jpeg_quality(value);
        }
}

