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

#include "util/Logger.h"
#include "cv/ImageIO.h"
#include "rpc/RemoteConfig.h"
#include "rpc/MethodsConfig.h"
#include "rpc/MethodsPowerDevice.h"

namespace romi {
        
        RemoteConfig::RemoteConfig(std::unique_ptr<rcom::IRPCClient>& client)
                : RemoteStub(client)
        {
        }
        
        bool RemoteConfig::has_section(const std::string& name)
        {
                nlohmann::json params;
                nlohmann::json result;
                params[MethodsConfig::kName] = name;

                bool success = execute("config", MethodsConfig::kHasSection,
                                       params, result);

                if (!success) {
                        r_err("RemoteConfig::has_section failed");
                        throw std::runtime_error("RemoteConfig::has_section failed");
                }

                return result[MethodsConfig::kAnswer];
        }
        
        nlohmann::json RemoteConfig::get_section(const std::string& name)
        {
                r_err("RemoteConfig::get_section");
                
                nlohmann::json params;
                nlohmann::json result;
                params[MethodsConfig::kName] = name;

                bool success = execute("config", MethodsConfig::kGetSection,
                                       params, result);

                if (!success) {
                        r_err("RemoteConfig::get_section failed");
                        throw std::runtime_error("RemoteConfig::get_section failed");
                }

                //std::cout << result << std::endl;
                
                return result;
        }
        
        nlohmann::json RemoteConfig::get()
        {
                nlohmann::json result;

                bool success = execute_with_result("config", MethodsConfig::kGet, result);
                if (!success) {
                        r_err("RemoteConfig::get failed");
                        throw std::runtime_error("RemoteConfig::get failed");
                }

                return result;
        }
        
        void RemoteConfig::set_section(const std::string& name,
                                       nlohmann::json& value)
        {
                nlohmann::json params;
                params[MethodsConfig::kName] = name;
                params[MethodsConfig::kValue] = value;

                bool success = execute_with_params(MethodsConfig::kSetSection, params);

                if (!success) {
                        r_err("RemoteConfig::set_section failed");
                        throw std::runtime_error("RemoteConfig::set_section failed");
                }
        }
}
