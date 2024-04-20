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

#include <stdlib.h>
#include <time.h>
#include <cstdlib>
#include "util/FileUtils.h"
#include "util/Logger.h"
#include "camera/FakeCamera.h"

namespace romi {

        FakeCamera::FakeCamera(size_t width, size_t height)
                : width_(width),
                  height_(height),
                  image_(Image::RGB, width, height),
                  jpeg_()
        {
                make_image();
                make_jpeg();
        }

        void FakeCamera::make_image()
        {
                srand((unsigned int) time(nullptr)); // Should not be done here
                std::vector<float>& buffer = image_.data();
                for (size_t i = 0; i < buffer.size(); i++) {
                        buffer[i] = (float) rand() / (float) RAND_MAX;
                }
        }

        void FakeCamera::make_jpeg()
        {
                std::vector<uint8_t> buffer;
                ImageIO::store_jpg_to_buffer(image_, buffer);
                jpeg_.append(buffer.data(), buffer.size());
        }

        bool FakeCamera::set_value(const std::string& name, double value)
        {
                (void) name;
                (void) value;
                return true;
        }
        
        bool FakeCamera::select_option(const std::string& name,
                                       const std::string& value)
        {
                (void) name;
                (void) value;
                return true;
        }
        
        bool FakeCamera::grab(Image &image)
        {
                image = image_;
                return true;
        }

        rcom::MemBuffer& FakeCamera::grab_jpeg()
        {
            return jpeg_;
        }

        bool FakeCamera::power_up()
        {
                return true;
        }
        
        bool FakeCamera::power_down()
        {
                return true;
        }
        
        bool FakeCamera::stand_by()
        {
                return true;
        }
        
        bool FakeCamera::wake_up()
        {
                return true;
        }

        const ICameraSettings& FakeCamera::get_settings()
        {
                throw std::runtime_error("FakeCamera::get_settings: not implemented");
        }
}

