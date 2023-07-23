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

#ifndef __ROMI_CAMERASETTINGS_H
#define __ROMI_CAMERASETTINGS_H

#include "camera/ICameraSettings.h"

namespace romi {
        
        class CameraSettings : public ICameraSettings
        {
        protected:
                std::string type_;
                nlohmann::json settings_;
                
        public:
                CameraSettings(const std::string& camera_type, nlohmann::json& json);
                ~CameraSettings() override = default;
                
                const std::string& type() const override;
                nlohmann::json get_all() override;

                double get_value(const std::string& name) override;
                bool set_value(const std::string& name, double value) override;
                void get_value_names(std::vector<std::string>& list) override;
                
                std::string& get_option(const std::string& name,
                                        std::string& value) override;
                bool select_option(const std::string& name,
                                   const std::string& value) override;
                void get_option_names(std::vector<std::string>& list) override;
        };
}

#endif // __ROMI_CAMERASETTINGS_H
