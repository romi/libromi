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

#include <stdexcept>
#include "cablebot/CablebotRunner.h"
#include "util/ClockAccessor.h"
#include "util/Logger.h"
#include "camera/CameraInfoIO.h"

namespace romi {

        CablebotRunner::CablebotRunner(std::shared_ptr<ICablebotProgramList>& programs,
                                       ImagingDevice& cablebot,
                                       Session& session,
                                       std::shared_ptr<romi::ICameraInfoIO>& info_io)
                : programs_(programs),
                  cablebot_(cablebot),
                  session_(session),
                  info_io_(info_io)
        {
        }
        
        void CablebotRunner::wakeup(uint8_t hour, uint8_t minute)
        {
                r_info("CablebotRunner::wakeup: %d:%d", (int) hour, (int) minute);
                try_run(hour, minute);
        }
        
        void CablebotRunner::try_run(uint8_t hour, uint8_t minute)
        {
                try {
                        auto program = programs_->find(hour, minute);
                        if (program)
                                run(*program);
                        //else throw std::runtime_error("Couldn't find the program");
                        
                } catch (std::exception& e) {
                        r_err("CablebotRunner::try_run: Failed: %s", e.what());
                        r_info("CablebotRunner::try_run: Trying to home");
                        try_finalize();
                }
        }
        
        void CablebotRunner::run(ICablebotProgram& program)
        {
                init(program);
                scan(program);
                finalize();
        }
        
        void CablebotRunner::init(ICablebotProgram& program)
        {
                r_debug("CablebotRunner::init: Scan starting");
                
                session_.start(program.observation_id());

                nlohmann::json json = info_io_->get();
                session_.store_metadata("camera", json);
                
                r_debug("CablebotRunner::init: Power up");
                cablebot_.power_up();
        }
        
        void CablebotRunner::scan(ICablebotProgram& program)
        {
                size_t counter = 0;
                double position = program.start();
                double end = program.start() + program.length();
                double tilt = program.tilt();
                auto clock = romi::ClockAccessor::GetInstance();

                r_debug("CablebotRunner::scan: Scanning");
        
                bool success = moveto(position, 0.0);
                if (!success) {
                        throw std::runtime_error("CablebotRunner::scan: Failed to "
                                                 "move to straing position");;
                }
                
                success = moveto(position, tilt);
                if (!success) {
                        throw std::runtime_error("CablebotRunner::scan: Failed to "
                                                 "move to straing position");;
                }
		
                while (true) {

                        clock->sleep(2.5); // Wait a wee until the cablebot stops moving
		
                        success = grab_image(counter++);
                        if (!success)
                                break;
                        
                        position += program.interval();
                        if (position > end + 0.005f)
                                break;
                                
                        success = moveto(position, tilt);
                        if (!success)
                                break;
                        
                }
        }
        
        void CablebotRunner::finalize()
        {
                r_debug("CablebotRunner::finalize");
                cablebot_.mount_->moveto(0.1, 0.0, 0.0,
                                         0.0, 0.0, 0.0,
                                         1.0);
                cablebot_.mount_->homing();
                cablebot_.power_down();
                session_.stop();
                r_debug("CablebotRunner::finalize: Scan done");
        }

        void CablebotRunner::try_finalize()
        {
                try {
                        finalize();
                } catch (std::exception& e) {
                        r_err("CablebotRunner::try_run: Homing failed: %s", e.what());                        
                }
        }

        bool CablebotRunner::moveto(double position, double angle)
        {
                r_debug("CablebotRunner::moveto: Move to %.2f", position);
                bool success = cablebot_.mount_->moveto(position, 0.0, 0.0,
                                                        angle, 0.0, 0.0,
                                                        1.0);
                if (!success) {
                        r_err("CablebotRunner::moveto: failed");
                }
                return success;
        }

        bool CablebotRunner::grab_image(size_t counter)
        {
                r_debug("CablebotRunner::grab_image");
                bool success = true;
                try {
                        rcom::MemBuffer& image = cablebot_.camera_->grab_jpeg();
                        success = store_image(image, counter);
                } catch (std::exception& e) {
                        r_err("CablebotRunner::grab_image: Failure: %s", e.what());
                        success = false;
                }
                return success;
        }

        bool CablebotRunner::store_image(rcom::MemBuffer& image, size_t counter)
        {
                r_debug("CablebotRunner::store_image");
                bool success = true;
                if (image.size() > 0) {
                        std::string filename = make_image_name(counter);
                        success = session_.store_jpg(filename, image);
                        if (!success) {
                                r_err("CablebotRunner::store_image: Failed to store the image");
                        } else {
                                r_debug("CablebotRunner::store_image: Stored %s", filename.c_str());
                        }
                }
                return success;
        }

        std::string CablebotRunner::make_image_name(size_t counter)
        {
                char buffer[64];
                snprintf(buffer, sizeof(buffer), "camera-%06zu.jpg", counter);
                return std::string(buffer);
        }

}
