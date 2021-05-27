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
#include <stdexcept>
#include <r.h>
#include "rover/RoverOptions.h"

using namespace std;

namespace romi {

    static std::vector<Option> options = {
                { "help", false, nullptr,
                  "Print help message" },

                // File paths
                
                { RoverOptions::config, true, "config.json",
                  "Path of the config file" },

                { RoverOptions::script, true, nullptr,
                  "The path of the file containing the scripts and menus" },
                
                { RoverOptions::registry, true, nullptr,
                  "The IP address of the registry"},
                
                { RoverOptions::session_directory, true, ".",
                  "The session directory where the output "
                  "files are stored (logs, images...)"},
                
                { RoverOptions::soundfont, true, nullptr,
                  "The path of the soundfont for the sound notification "},

                { RoverOptions::joystick_device, true, nullptr,
                  "The path of the system device for the input device" },
                
                { RoverOptions::display_device, true, nullptr,
                  "The path of the system device for the display "},
                
                { RoverOptions::camera_classname, true, nullptr,
                  "The classname of the camera to instanciate."},
                
                { RoverOptions::camera_device, true, nullptr,
                  "The device path for the USB camera."},
                
                { RoverOptions::camera_image, true, nullptr,
                  "The path of the image file for the file camera."},
                
                { RoverOptions::cnc_device, true, nullptr,
                  "The stepper controller's serial device "},
                
                { RoverOptions::navigation_device, true, nullptr,
                  "The brush motor driver's serial device"
                },
        };

        RoverOptions::RoverOptions()
            : GetOpt(options)
        {}
        
        void RoverOptions::exit_if_help_requested()
        {
                if (is_help_requested()) {
                        print_usage();
                        exit(0);
                }
        }

    std::string RoverOptions::get_config_file()
    {
            std::string file = get_value(romi::RoverOptions::config);
            if (file.empty()) {
                    throw std::runtime_error("No configuration file was given (can't run without one...).");
            }
            return file;
    }

}

