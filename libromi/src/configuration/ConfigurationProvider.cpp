/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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
#include "util/Logger.h"
#include "configuration/ConfigurationProvider.h"

namespace romi {

    std::string get_brush_motor_device_in_config(nlohmann::json &config)
    {
            std::string brush_motor_device;
            try {
                    brush_motor_device = config["ports"]["brush-motor-driver"]["port"];

            } catch (nlohmann::json::exception& je) {
                    r_warn("Failed to get the value for "
                           "ports.brush_motor.port: %s", je.what());
                    throw std::runtime_error("No brush_motor device specified");
            }
            return brush_motor_device;
    }

    std::string get_brush_motor_device(romi::IOptions &options, nlohmann::json &config)
    {
            std::string brush_motor_device
                    = options.get_value(romi::RoverOptions::kNavigationDevice);
            if (brush_motor_device.empty()) {
                    brush_motor_device = get_brush_motor_device_in_config(config);
            }
            return brush_motor_device;
    }

    // std::string get_sound_font_in_config(nlohmann::json &config)
    // {
    //         try {
    //                 return config["user-interface"]["fluid-sounds"][RoverOptions::soundfont];

    //         } catch (nlohmann::json::exception& je) {
    //                 r_err("FluidSoundNotification: Failed to read the config: %s",
    //                       je.what());
    //                 throw std::runtime_error("No soundfont in the config file");
    //         }
    // }

    // std::string get_sound_font_file(romi::IOptions &options, nlohmann::json &config)
    // {
    //         std::string file = options.get_value(RoverOptions::soundfont);
    //         if (file.empty())
    //                 file = get_sound_font_in_config(config);
    //         return file;
    // }

    // std::string get_script_file_in_config(nlohmann::json &config)
    // {
    //         try {
    //                 return config["user-interface"]["script-engine"][RoverOptions::script];

    //         } catch (nlohmann::json::exception& je) {
    //                 r_err("Failed to read script file in config: %s",
    //                       je.what());
    //                 throw std::runtime_error("No script file in the config file");
    //         }
    // }

    std::string get_script_file(romi::IOptions &options, nlohmann::json&)
    {
            std::string file = options.get_value(romi::RoverOptions::kScript);
            // if (file.empty()) {
            //         file = get_script_file_in_config(config);
            // }
            return file;
    }

    std::string get_session_directory(romi::IOptions &options, nlohmann::json &config)
    {
            (void) config;
            std::string dir = options.get_value(romi::RoverOptions::kSessionDirectory);
            if (dir.empty()) {
                    // TODO: to be finalized: get seesion dir from config
                    // file, add the current date to the path, and create
                    // a new directory.
                    dir = ".";
            }
            return dir;
    }

    std::string get_camera_image(romi::IOptions& options, nlohmann::json& config)
    {
            std::string path = options.get_value(romi::RoverOptions::kCameraImage);
            if (path.empty()) {
                    path = config["weeder"]["file-camera"]["image"];
            }
            return path;
    }

    std::string get_camera_device_in_config(nlohmann::json& config)
    {
            try {
                    return config["ports"]["usb-camera"]["port"];

            } catch (nlohmann::json::exception& je) {
                    r_err("get_camera_device_in_config: Failed to get value "
                          "of ports.usb-camera.port");
                    throw std::runtime_error("Missing device name for camera in config");
            }
    }

    std::string get_camera_device(romi::IOptions& options, nlohmann::json& config)
    {
            std::string device = options.get_value(romi::RoverOptions::kCameraDevice);
            if (device.empty())
                    device = get_camera_device_in_config(config);
            return device;
    }

    std::string get_camera_classname_in_config(nlohmann::json& config)
    {
            try {
                    return config["weeder"]["camera-classname"];

            } catch (nlohmann::json::exception& je) {
                    r_err("get_camera_classname_in_config: Failed to get value "
                          "of weeder.camera-classname");
                    throw std::runtime_error("Missing classname for camera in config");
            }
    }

    std::string get_camera_classname(romi::IOptions& options, nlohmann::json& config)
    {
            std::string device = options.get_value(romi::RoverOptions::kCameraClassname);
            if (device.empty())
                    device = get_camera_classname_in_config(config);
            return device;
    }
}
