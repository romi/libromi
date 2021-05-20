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
#ifndef ROMI_ROVER_BUILD_AND_TEST_SESSION_H
#define ROMI_ROVER_BUILD_AND_TEST_SESSION_H

#include <filesystem>
#include "ILinux.h"
#include "data_provider/IRomiDeviceData.h"
#include "data_provider/ISoftwareVersion.h"
#include "data_provider/ILocationProvider.h"
#include "session/MetaFolder.h"
#include "ISession.h"

namespace romi {

    class Session : public ISession {
    public:
        Session() = delete;

        explicit Session(rpp::ILinux& linux, const std::string& base_directory,
                         IRomiDeviceData& device_data, ISoftwareVersion& softwareVersion,
                         std::unique_ptr<ILocationProvider> location);
        ~Session() override = default;
        void start(const std::string& observation_id) override;
        void stop() override;
        bool store_jpg(const std::string& name, Image& image)  override;
        bool store_jpg(const std::string& name, rpp::MemBuffer& jpeg) override;
        bool store_png(const std::string& name, Image& image)  override;
        bool store_svg(const std::string& name, const std::string& body) override;
        bool store_txt(const std::string& name, const std::string& body) override;
        bool store_path(const std::string& filename, int32_t path_number, Path& weeder_path) override;
        std::filesystem::path current_path() override;

    private:
        std::filesystem::path base_directory_;
        std::filesystem::path session_directory_;
        IRomiDeviceData& device_data_;
        std::shared_ptr<IMetaFolder> meta_folder_;
        std::string observation_id_;

    };
}

#endif //ROMI_ROVER_BUILD_AND_TEST_SESSION_H
