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

#ifndef __ROMI_FAKECAMERA_H
#define __ROMI_FAKECAMERA_H

#include <string>
#include <stdexcept>
#include "cv/ImageIO.h"
#include "api/ICamera.h"

namespace romi {

        class FakeCamera : public ICamera
        {
        public:
                static constexpr const char *ClassName = "fake-camera";
                
        protected:
                size_t width_;
                size_t height_;
                Image image_;
                rcom::MemBuffer jpeg_;
                
                void make_image();
                void make_jpeg();
                
        public:
                
                explicit FakeCamera(size_t width, size_t height);
                ~FakeCamera() override = default;
        
                bool grab(Image &image) override;
                rcom::MemBuffer& grab_jpeg() override;
                
                bool set_value(const std::string& name, double value) override;
                bool select_option(const std::string& name,
                                   const std::string& value) override;
                const ICameraSettings& get_settings() override;

                // Power device interface
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_FAKECAMERA_H
