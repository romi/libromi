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
#ifndef __ROMI_CABLEBOTPROGRAMIO_H
#define __ROMI_CABLEBOTPROGRAMIO_H

#include <rcom/json.hpp>
#include "cablebot/ICablebotProgramIO.h"
#include "api/IConfigManager.h"

namespace romi {

        class CablebotProgramIO : public ICablebotProgramIO
        {
        public:
                static const constexpr char *kID = "id";
                static const constexpr char *kName = "name";
                static const constexpr char *kObservationId = "observation-id";
                static const constexpr char *kHour = "hour";
                static const constexpr char *kMinute = "minute";
                static const constexpr char *kStart = "start-at";
                static const constexpr char *kLength = "length";
                static const constexpr char *kInterval = "interval";
                static const constexpr char *kTilt = "tilt";
                static const constexpr char *kEnabled = "enabled";
                
        protected:
                std::shared_ptr<IConfigManager> config_;
                const std::string section_;

                static nlohmann::json convert(std::shared_ptr<ICablebotProgram>& program);
                
        public:
                CablebotProgramIO(std::shared_ptr<IConfigManager>& config,
                                  const std::string& section);
                ~CablebotProgramIO() override = default;
                
                std::unique_ptr<ICablebotProgramList> load() override;
                void store(ICablebotProgramList& info) override;
                
                static nlohmann::json to_json(ICablebotProgramList& info);
                static std::unique_ptr<ICablebotProgramList> from_json(nlohmann::json& json);
                static std::shared_ptr<ICablebotProgram> load_program(nlohmann::json json);
        };

}

#endif // __ROMI_CABLEBOTPROGRAMIO_H
