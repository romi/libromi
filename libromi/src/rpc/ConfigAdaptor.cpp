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
#include "rpc/ConfigAdaptor.h"
#include "rpc/MethodsConfig.h"

namespace romi {

        ConfigAdaptor::ConfigAdaptor(IConfigManager& config)
                : config_(config)
        {
        }

        void ConfigAdaptor::execute(const std::string&,
                                    const std::string&,
                                    nlohmann::json&,
                                    rcom::MemBuffer&,
                                    rcom::RPCError& error)
        {
                error.code = rcom::RPCError::kMethodNotFound;
                error.message = "Unknown method";
        }

        
        void ConfigAdaptor::execute(const std::string&,
                                    const std::string& method,
                                    nlohmann::json& params,
                                    nlohmann::json& result,
                                    rcom::RPCError& error)
        {
                try {
                        if (method == MethodsConfig::kGet) {
                                execute_get(result);
                                
                        } else if (method == MethodsConfig::kGetSection) {
                                execute_get_section(params, result, error);
                                
                        } else if (method == MethodsConfig::kHasSection) {
                                execute_has_section(params, result, error);
                                
                        } else if (method == MethodsConfig::kSetSection) {
                                execute_set_section(params, error);
                                
                        } else {
                                error.code = rcom::RPCError::kMethodNotFound;
                                error.message = "Unknown method";
                        }
                        
                } catch (std::exception& e) {
                        error.code = rcom::RPCError::kInternalError;
                        error.message = e.what();
                }
        }

        void ConfigAdaptor::execute_get(nlohmann::json& result)
        {
                result = config_.get();
        }

        void ConfigAdaptor::execute_get_section(nlohmann::json& params,
                                                nlohmann::json& result,
                                                rcom::RPCError& error)
        {
                if (!assure_param(params, MethodsConfig::kName, error)) {
                        return;
                }
                
                std::string name = params[MethodsConfig::kName];
                // TODO: check if the name is valid
                if (config_.has_section(name)) {
                        r_info("ConfigAdaptor::execute_get_section: %s", name.c_str());
                        result = config_.get_section(name);
                } else {
                        r_err("ConfigAdaptor::execute_get_section: no section '%s'",
                              name.c_str());
                        error.code = 1;
                        error.message = "get_section: bad section name";
                }
        }

        void ConfigAdaptor::execute_has_section(nlohmann::json& params,
                                                nlohmann::json& result,
                                                rcom::RPCError& error)
        {
                if (!assure_param(params, MethodsConfig::kName, error)) {
                        return;
                } 

                std::string name = params[MethodsConfig::kName];
                // TODO: check if the name is valid
                result[MethodsConfig::kAnswer] = config_.has_section(name);
        }

        void ConfigAdaptor::execute_set_section(nlohmann::json& params,
                                                rcom::RPCError& error)
        {
                if (!assure_param(params, MethodsConfig::kName, error)
                    || !assure_param(params, MethodsConfig::kValue, error)) {
                        return;
                } 

                std::string name = params[MethodsConfig::kName];
                nlohmann::json value = params[MethodsConfig::kValue];
                
                // TODO: check if the name is valid
                if (config_.has_section(name)) {
                        r_info("ConfigAdaptor::execute_set_section: %s", name.c_str());
                        config_.set_section(name, value);
                } else {
                        r_err("ConfigAdaptor::execute_set_section: no section '%s'",
                              name.c_str());
                        error.code = 1;
                        error.message = "get_section: bad section name";
                }
        }

        bool ConfigAdaptor::assure_param(nlohmann::json& params,
                                      const std::string& name,
                                      rcom::RPCError& error)
        {
                bool success = false;
                if (!params.contains(name)) {
                        r_err("ConfigAdaptor::assure_param_section: missing parameter %s",
                              name.c_str());
                        error.code = rcom::RPCError::kInvalidParams;
                        error.message = "Missing parameter";
                } else {
                        success = true;
                        error.code = 0;
                        error.message = "";
                }
                return success;
        }
}
