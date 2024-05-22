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

#ifndef __ROMI_USB_CAMERA_H
#define __ROMI_USB_CAMERA_H

#include <string>
#include <stdexcept>
#include <mutex>
#include <thread>
#include <json.hpp>

#include "rcom/MemBuffer.h"
#include "api/ICamera.h"
#include "cv/Image.h"
#include "util/Logger.h"
#include <atomic>

typedef struct _camera_t camera_t;

namespace romi {

        class USBCamera : public ICamera
        {
        public:
                static constexpr const char *ClassName = "usb-camera";

        protected:
                camera_t* _camera;
                std::string _device;
                std::mutex _mutex;
                std::atomic<bool> _done;
                Image _image;
                rcom::MemBuffer _buffer;
                std::thread _thread;
                
                bool open(size_t width, size_t height);
                void grab_from_camera();
                void start_capture_thread();
                void run();


        public:
                
                USBCamera(const std::string& device, size_t width, size_t height);
                USBCamera(const USBCamera&) = delete;
                USBCamera& operator=(const USBCamera&) = delete;
                ~USBCamera() override;
                
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

#endif // __ROMI_USB_CAMERA_H
