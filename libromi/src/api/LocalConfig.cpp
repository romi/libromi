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

#include <fstream>
#include "api/LocalConfig.h"
#include "util/Logger.h"

namespace romi {

        LocalConfig::LocalConfig(std::filesystem::path& path)
                : path_(path),
                  config_()
        {
                load();
        }
        
        void LocalConfig::load()
        {
                std::ifstream ifs(path_);
                config_ = nlohmann::json::parse(ifs);
        }
        
        void LocalConfig::store()
        {
                std::ofstream ofs(path_);
                ofs << config_.dump(4) << std::endl;
        }
                
        bool LocalConfig::has_section(const std::string& name)
        {
                return config_.contains(name);
        }
                
        void LocalConfig::set_section(const std::string& name,
                                        nlohmann::json& value)
        {
                config_[name] = value;
                store();
        }
        
        nlohmann::json LocalConfig::get_section(const std::string& name)
        {
                return config_[name];
        }
        
        nlohmann::json LocalConfig::get()
        {
                return config_;
        }
}
