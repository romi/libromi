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
#include <errno.h>
#include <sys/wait.h>
#include "util/FileUtils.h"
#include "util/Logger.h"
#include "camera/ExternalCamera.h"

namespace romi {

        ExternalCamera::ExternalCamera(const std::string& executable)
                : executable_(executable),
                  image_(),
                  jpeg_()
        {
                if (executable_.length() == 0)
                        throw std::runtime_error("ExternalCamera: Invalid executable");
        }

        bool ExternalCamera::load_image()
        {
                return ImageIO::load(image_, kImagePath);
        }

        void ExternalCamera::load_jpeg()
        {
            std::vector<uint8_t> image_data;
            jpeg_.clear();
            FileUtils::TryReadFileAsVector(kImagePath, image_data);
            jpeg_.append(image_data.data(), image_data.size());
        }

        bool ExternalCamera::set_value(const std::string& name, double value)
        {
                (void) name;
                (void) value;
                return true;
        }
        
        bool ExternalCamera::select_option(const std::string& name,
                                       const std::string& value)
        {
                (void) name;
                (void) value;
                return true;
        }
        
        bool ExternalCamera::grab(Image &image)
        {
                image = image_;
                return true;
        }

        rcom::MemBuffer& ExternalCamera::grab_jpeg()
        {
                execute();
                load_jpeg();
                return jpeg_;
        }

        void ExternalCamera::execute()
        {
                char command[2048];
                snprintf(command, sizeof(command), "%s %s",
                         executable_.c_str(), kImagePath);
                command[2047] = 0;
                
                int status = system(command);
                if (status == -1) {
                        throw std::runtime_error("Failed to create the child process");
                } else {
                        int ret = WEXITSTATUS(status);
                        if (ret != 0) {
                                throw std::runtime_error("The child process failed");
                        }
                }
        }

        bool ExternalCamera::power_up()
        {
                return true;
        }
        
        bool ExternalCamera::power_down()
        {
                return true;
        }
        
        bool ExternalCamera::stand_by()
        {
                return true;
        }
        
        bool ExternalCamera::wake_up()
        {
                return true;
        }

        const ICameraSettings& ExternalCamera::get_settings()
        {
                throw std::runtime_error("ExternalCamera::get_settings: not implemented");
        }
}

