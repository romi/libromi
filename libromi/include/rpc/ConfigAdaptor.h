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
#ifndef __ROMI_CONFIGADAPTOR_H
#define __ROMI_CONFIGADAPTOR_H

#include <rcom/IRPCHandler.h>
#include "api/IConfigManager.h"

namespace romi {

        class ConfigAdaptor : public rcom::IRPCHandler
        {
        protected:
                IConfigManager& config_;

                void assure_param(nlohmann::json& params,
                                  const std::string& name);
                void execute_get(nlohmann::json& result);
                void execute_get_section(nlohmann::json& params,
                                         nlohmann::json& result,
                                         rcom::RPCError& error);
                void execute_has_section(nlohmann::json& params,
                                         nlohmann::json& result,
                                         rcom::RPCError& error);
                void execute_set_section(nlohmann::json& params,
                                         rcom::RPCError& error);
                bool assure_param(nlohmann::json& params,
                                  const std::string& name,
                                  rcom::RPCError& error);
                
        public:
                ConfigAdaptor(IConfigManager& config);
                ~ConfigAdaptor() override = default;
        
                void execute(const std::string& id,
                             const std::string& method,
                             nlohmann::json& params,
                             nlohmann::json& result,
                             rcom::RPCError& status) override;
                void execute(const std::string& id,
                             const std::string& method,
                             nlohmann::json& params,
                             rcom::MemBuffer& result,
                             rcom::RPCError &status) override;
        };
}

#endif // __ROMI_CONFIGADAPTOR_H
