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

#ifndef __ROMI_LIBCAMERA_H
#define __ROMI_LIBCAMERA_H

#include <string>
#include <stdexcept>
#include <mutex>
#include <condition_variable>
#include "cv/ImageIO.h"
#include "api/ICamera.h"
#include <libcamera/libcamera.h>

namespace romi {

        class LibCamera : public ICamera
        {
        public:
                static constexpr const char *ClassName = "libcamera";
                
        protected:
                std::unique_ptr<libcamera::CameraManager> manager_;
                std::shared_ptr<libcamera::Camera> camera_;
                libcamera::FrameBufferAllocator *allocator_;
                libcamera::Stream *stream_;
                std::unique_ptr<libcamera::Request> request_;
                libcamera::PixelFormat pixel_format_;
                size_t width_;
                size_t height_;
                std::mutex mutex_;
                std::condition_variable cv_;
                bool request_completed_;
                Image image_;
                rcom::MemBuffer jpeg_;
                
        public:
                
                explicit LibCamera();
                ~LibCamera() override;
        
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

        protected:
                void assert_format();
                void send_request();
                void wait_request_completed();
                void signal_request_completed();
                void request_complete(libcamera::Request *request);
                void process_request_buffer(libcamera::Request *request);
                void import_data(const uint8_t *data, size_t length);
                void import_jpeg(const uint8_t *data, size_t length);
                void import_rgb(const uint8_t *data, size_t length);
                void convert_jpeg_to_rgb_perhaps();
                void convert_jpeg_to_rgb();
                void convert_rgb_to_jpeg_perhaps();
                void convert_rgb_to_jpeg();
        };
}

#endif // __ROMI_LIBCAMERA_H
