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

#ifndef __ROMI_PYTHON_TRACK_FOLLOWER_H
#define __ROMI_PYTHON_TRACK_FOLLOWER_H

#include <string> 
#include <rpc/RcomClient.h>
#include "api/ICamera.h"
#include "session/ISession.h"
#include "rover/ITrackFollower.h"

namespace romi {

        class PythonTrackFollower : public ITrackFollower
        {
        protected:

                static constexpr const char *kImageName = "navigation-image";
                static constexpr const char *kCrossTrackErrorKey = "cross-track-error";
                static constexpr const char *kOrientationErrorKey = "orientation-error";

                ICamera& camera_;
                std::unique_ptr<romi::IRPCClient> rpc_;
                std::string function_name_;
                ISession& session_;
                double cross_track_error_;
                double orientation_error_;
                size_t image_counter_;
                
                void try_update();
                rpp::MemBuffer& grab_image();
                std::string get_image_name();
                std::string get_image_path(std::string& filename);
                void store_image(rpp::MemBuffer& jpg, std::string& path);
                JsonCpp send_python_request(const std::string& path);
                void parse_response(JsonCpp& response);
                
        public:
                PythonTrackFollower(ICamera& camera,
                                    std::unique_ptr<romi::IRPCClient>& rpc,
                                    const std::string& function_name,
                                    ISession& session);
                ~PythonTrackFollower() override = default;
                
                void start() override;
                bool update_error_estimate() override;
                double get_cross_track_error() override;
                double get_orientation_error() override;
        };
}

#endif // __ROMI_PYTHON_TRACK_FOLLOWER_H
