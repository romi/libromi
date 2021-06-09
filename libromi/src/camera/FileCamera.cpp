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

#include <FileUtils.h>
#include "camera/FileCamera.h"

namespace romi {

        FileCamera::FileCamera(const std::string& filename)
                : filename_(filename), image_(), filebuffer_()
        {
                if (filename_.length() == 0)
                        throw std::runtime_error("FileCamera: Invalid filename");
                
                if (!open()) {
                        r_err("Failed to load the file: %s", filename_.c_str());
                        throw std::runtime_error("FileCamera::open failed");
                }
        }

        bool FileCamera::open()
        {
                return ImageIO::load(image_, filename_.c_str());
        }
        
        bool FileCamera::grab(Image &image)
        {
                image = image_;
                return true;
        }

        rpp::MemBuffer& FileCamera::grab_jpeg() {
            std::vector<uint8_t > image_data;
            filebuffer_.clear();
            FileUtils::TryReadFileAsVector(filename_, image_data);
            filebuffer_.append(image_data.data(), image_data.size());
            return filebuffer_;
        }
}

