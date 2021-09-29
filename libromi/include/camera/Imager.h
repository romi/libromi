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

#ifndef __ROMI_IMAGER_H
#define __ROMI_IMAGER_H

#include <atomic>
#include <thread>
#include <memory>
#include "session/ISession.h"
#include "api/ICamera.h"
#include "api/IImager.h"

namespace romi {
        
        class Imager : public IImager
        {
        public:
                static const size_t kDefaultMaxImages = 10000;
                static constexpr double kDefaultMaxDuration = 10.0 * 60.0; // 10 minutes or 600 s
                
        protected:
                ISession& session_;
                ICamera& camera_;
                size_t max_images_;
                double max_duration_;
                std::atomic<bool> recording_;
                size_t counter_;
                std::unique_ptr<std::thread> thread_;
                
                void try_start_recording(const std::string& observation_id,
                                         size_t max_images,
                                         double max_duration);
                void assert_not_recording();
                void grab_loop();
                virtual bool grab();
                std::string make_image_name();

        public:
                Imager(ISession& session, ICamera& camera);
                ~Imager() override;
                
                bool start_recording(const std::string& observation_id,
                                     size_t max_images,
                                     double max_duration) override;
                bool stop_recording() override;
                bool is_recording()override;
        };
}

#endif // __ROMI_IMAGER_H
