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

#ifndef __ROMI_ICONFIGMANAGER_H
#define __ROMI_ICONFIGMANAGER_H

#include "json.hpp"

namespace romi {

        class IConfigManager
        {
            public:
                virtual ~IConfigManager() = default;
                virtual bool has_section(const std::string& name) = 0;
                virtual void set_section(const std::string& name,
                                         nlohmann::json& value) = 0;
                virtual nlohmann::json get_section(const std::string& name) = 0;
                virtual nlohmann::json get() = 0;
        };
}

#endif // __ROMI_ICONFIGMANAGER_H
