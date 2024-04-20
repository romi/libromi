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

#include "camera/CameraWithConfig.h"
#include "util/Logger.h"

namespace romi {

        CameraWithConfig::CameraWithConfig(std::shared_ptr<ICameraInfoIO>& io,
                                           std::unique_ptr<ICamera>& camera)
                : io_(io),
                  info_(),
                  camera_(std::move(camera))
        {
                info_ = io_->load();
                apply_settings();
        }

        void CameraWithConfig::apply_settings()
        {
                apply_values();
                apply_options();
        }

        void CameraWithConfig::apply_values()
        {
                ICameraSettings& settings = info_->get_settings();
                std::vector<std::string> names;
                settings.get_value_names(names);
                for (size_t i = 0; i < names.size(); i++) {
                        double value = settings.get_value(names[i]);
                        r_debug("CameraWithConfig: set_value('%s', %f)",
                                names[i].c_str(), value);
                        camera_->set_value(names[i], value);
                }
        }

        void CameraWithConfig::apply_options()
        {
                std::string value;
                ICameraSettings& settings = info_->get_settings();
                std::vector<std::string> names;
                settings.get_option_names(names);
                for (size_t i = 0; i < names.size(); i++) {
                        settings.get_option(names[i], value);
                        r_debug("CameraWithConfig: set_option('%s', '%s')",
                                names[i].c_str(), value.c_str());
                        camera_->select_option(names[i], value);
                }                
        }

        bool CameraWithConfig::set_value(const std::string& name, double value)
        {
                bool success = camera_->set_value(name, value);
                if (success) {
                        ICameraSettings& settings = info_->get_settings();
                        success = settings.set_value(name, value);
                        if (success) {
                                io_->store(*info_);
                        }
                }
                return success;
        }

        bool CameraWithConfig::select_option(const std::string& name,
                                             const std::string& value)
        {
                bool success = camera_->select_option(name, value);
                if (success) {
                        ICameraSettings& settings = info_->get_settings();
                        success = settings.select_option(name, value);
                        if (success) {
                                io_->store(*info_);
                        }
                }
                return success;
        }
        
        bool CameraWithConfig::grab(Image &image)
        {
                return camera_->grab(image);
        }

        rcom::MemBuffer& CameraWithConfig::grab_jpeg()
        {
                return camera_->grab_jpeg();
        }

        bool CameraWithConfig::power_up()
        {
                return camera_->power_up();
        }
        
        bool CameraWithConfig::power_down()
        {
                return camera_->power_down();
        }
        
        bool CameraWithConfig::stand_by()
        {
                return camera_->stand_by();
        }
        
        bool CameraWithConfig::wake_up()
        {
                return camera_->wake_up();
        }

        const ICameraSettings& CameraWithConfig::get_settings()
        {
                return info_->get_settings();
        }
}

