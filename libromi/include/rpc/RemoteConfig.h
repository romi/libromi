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
#ifndef __ROMI_REMOTECONFIG_H
#define __ROMI_REMOTECONFIG_H

#include <memory>
#include <rcom/RemoteStub.h>
#include "api/IConfigManager.h"

namespace romi {

        class RemoteConfig : public IConfigManager, public rcom::RemoteStub
        {
        public:
                static constexpr const char *ClassName = "remote-config";

        public:
                RemoteConfig(std::unique_ptr<rcom::IRPCClient>& client);
                ~RemoteConfig() override = default;

                bool has_section(const std::string& name) override;
                void set_section(const std::string& name, nlohmann::json& value) override;
                nlohmann::json get_section(const std::string& name) override;
                nlohmann::json get() override;
        };
}

#endif // __ROMI_REMOTECONFIG_H
