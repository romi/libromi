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
#ifndef __ROMI_CAMERAADAPTOR_H
#define __ROMI_CAMERAADAPTOR_H

#include <rcom/IRPCHandler.h>
#include "api/ICamera.h"

namespace romi {

        class CameraAdaptor : public rcom::IRPCHandler
        {
        protected:
                ICamera& camera_;

                void execute_power_up(rcom::RPCError& error);
                void execute_power_down(rcom::RPCError& error);
                void execute_stand_by(rcom::RPCError& error);
                void execute_wake_up(rcom::RPCError& error);
                void execute_set_value(nlohmann::json& params,
                                       nlohmann::json& result,
                                       rcom::RPCError& error);
                void execute_select_option(nlohmann::json& params,
                                           nlohmann::json& result,
                                           rcom::RPCError& error);
                
        public:
                CameraAdaptor(ICamera& camera);
                ~CameraAdaptor() override = default;
        
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

#endif // __ROMI_CAMERAADAPTOR_H
