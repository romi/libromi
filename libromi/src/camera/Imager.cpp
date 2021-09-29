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

#include <functional>
#include <ClockAccessor.h>
#include "camera/Imager.h"

namespace romi {
        
        Imager::Imager(ISession& session, ICamera& camera)
                : session_(session),
                  camera_(camera),
                  max_images_(kDefaultMaxImages),
                  max_duration_(kDefaultMaxDuration),
                  recording_(false),
                  counter_(0),
                  thread_()
        {
        }

        Imager::~Imager()
        {
                stop_recording();
        }

        bool Imager::start_recording(const std::string& observation_id,
                                     size_t max_images,
                                     double max_duration)
        {
                counter_ = 0;
                bool success = false;
                try {
                        try_start_recording(observation_id, max_images,
                                            max_duration);
                        success = true;
                        
                } catch (std::exception& e) {
                        r_err("Imager::start_recording: Failure: %s", e.what());
                }
                return success;
        }
                
        void Imager::try_start_recording(const std::string& observation_id,
                                         size_t max_images,
                                         double max_duration)
        {
                assert_not_recording();
                if (max_duration_ > 0.0 && max_images_ > 0) {
                        max_images_ = max_images;
                        max_duration_ = max_duration;
                        session_.start(observation_id);
                        recording_ = true;
                        thread_ = std::make_unique<std::thread>([this] { grab_loop(); });
                }
        }
                
        void Imager::assert_not_recording()
        {
                if (recording_ || thread_) {
                        throw std::runtime_error("Already recording");
                }
        }
                        
        bool Imager::stop_recording()
        {
                if (recording_) {
                        recording_ = false;
                }
                if (thread_) {
                        thread_->join();
                        thread_ = nullptr;
                }
                return true;
        }

        bool Imager::is_recording()
        {
            return recording_;
        }
        void Imager::grab_loop()
        {
                double start_time = rpp::ClockAccessor::GetInstance()->time();
                
                while (recording_) {

                        if (!grab()) {
                                r_warn("Imager::grab_loop: grab failed. Trying to continue.");
                        }
                        
                        counter_++;
                        if (counter_ >= max_images_) {
                                r_warn("Imager::grab_loop: reach maximum number "
                                       "of images before stop was called");
                                recording_ = false;
                        }
                        
                        double now = rpp::ClockAccessor::GetInstance()->time();
                        if (now - start_time > max_duration_) {
                                r_warn("Imager::grab_loop: reach maximum duration "
                                       "before stop was called");
                                recording_ = false;
                        }
                }
                
                session_.stop();
        }
        
        bool Imager::grab()
        {
                bool success = false;
                rpp::MemBuffer& jpeg = camera_.grab_jpeg();
                if (jpeg.size() > 0) {
                        std::string filename = make_image_name();
                        success = session_.store_jpg(filename, jpeg);
                }
                return success;
        }

        std::string Imager::make_image_name()
        {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "camera-%06zu.jpg", counter_);
                return std::string(buffer);
        }
}
