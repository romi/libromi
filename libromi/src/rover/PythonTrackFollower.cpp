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

#include "rover/PythonTrackFollower.h"

namespace romi {

        PythonTrackFollower::PythonTrackFollower(ICamera& camera,
                                                 std::unique_ptr<romi::IRPCClient>& rpc,
                                                 const std::string& function_name,
                                                 ISession& session)
                : camera_(camera),
                  rpc_(),
                  function_name_(function_name),
                  session_(session),
                  cross_track_error_(0.0),
                  orientation_error_(0.0),
                  image_counter_(0)
        {
                //rpc_ = romi::RcomClient::create("python", 30);
                rpc_ = std::move(rpc);
        }

        void PythonTrackFollower::start()
        {
        }
        
        double PythonTrackFollower::get_cross_track_error()
        {
                return cross_track_error_;
        }
        
        double PythonTrackFollower::get_orientation_error()
        {
                return orientation_error_;
        }
        
        bool PythonTrackFollower::update_error_estimate()
        {
                bool success = false;
                try {
                        try_update();
                        success = true;
                } catch (const std::exception& e) {
                        r_err("PythonTrackFollower::update_error_estimate: caught: %s",
                              e.what());
                }
                return success;
        }
        
        void PythonTrackFollower::try_update()
        {
                auto filename = get_image_name();
                auto image = grab_image();
                store_image(image, filename);
                auto path = get_image_path(filename);
                JsonCpp response = send_python_request(path);
                parse_response(response);
        }

        rpp::MemBuffer& PythonTrackFollower::grab_image()
        {
                image_counter_++;
                return camera_.grab_jpeg();
        }

        std::string PythonTrackFollower::get_image_name()
        {
                char filename[64];
                snprintf(filename, sizeof(filename), "%s-%06zu.jpg",
                         kImageName, image_counter_);
                return std::string(filename);
        }

        std::string PythonTrackFollower::get_image_path(std::string& name)
        {
                std::filesystem::path dir = session_.current_path();
                std::filesystem::path path = dir /= name;
                return path.string();
        }

        void PythonTrackFollower::store_image(rpp::MemBuffer& jpg, std::string& name)
        {
                if (!session_.store_jpg(name, jpg)) {
                        r_err("PythonTrackFollower::store_image: store_jpg failed");
                        throw std::runtime_error("Failed to save the image");
                }
        }

        JsonCpp PythonTrackFollower::send_python_request(const std::string& path)
        {
                JsonCpp response;
                romi::RPCError error;
                
                JsonCpp params = JsonCpp::construct("{\"path\": \"%s\"}", path.c_str());
        
                rpc_->execute(function_name_, params, response, error);
                
                if (error.code != 0) {
                        r_warn("Failed to call Python: %s", error.message.c_str());
                        throw std::runtime_error("Failed to call Python");
                        
                } else if (response.get("error").num("code") != 0) {
                        const char *message = response.get("error").str("message");
                        r_warn("Failed to call Python: %s", message);
                        throw std::runtime_error("Failed to call Python");
                }

                return response;
        }
                
        void PythonTrackFollower::parse_response(JsonCpp& response)
        {
                cross_track_error_ = response.num(kCrossTrackErrorKey);
                orientation_error_ = response.num(kOrientationErrorKey);
        }
}
