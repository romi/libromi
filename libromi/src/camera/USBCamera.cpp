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

#include <chrono>
#include "camera_v4l.h"
#include "camera/USBCamera.h"
#include "cv/ImageIO.h"
#include "util/Logger.h"

namespace romi {
        
        using SynchonizedCodeBlock = std::lock_guard<std::mutex>;

        USBCamera::USBCamera(const std::string& device, size_t width, size_t height)
                : _camera(nullptr),
                  _device(device),
                  _mutex(),
                  _done(false),
                  _image(),
                  _buffer(),
                  _thread()
        {
                if (_device.length() == 0)
                        throw std::runtime_error("USBCamera: Invalid device");
                        
                if (width <= 10 || width > 10000)
                        throw std::runtime_error("USBCamera: Invalid width");
                        
                if (height <= 10 || height > 10000)
                        throw std::runtime_error("USBCamera: Invalid height");
                
                if (!open(width, height))
                        throw std::runtime_error("USBCamera::open failed");
        }

        USBCamera::~USBCamera()
        {
                _done = true;
                if (_thread.joinable()) _thread.join();

                if (_camera)
                        delete_camera(_camera);
        }

        bool USBCamera::set_value(const std::string& name, double value)
        {
                (void) name;
                (void) value;
                r_warn("USBCamera::set_value::Not implemented");
                //throw std::runtime_error("USBCamera::set_value::Not implemented");
                return true;
        }
        
        bool USBCamera::select_option(const std::string& name,
                                      const std::string& value)
        {
                (void) name;
                (void) value;
                r_warn("USBCamera::set_value::Not implemented");
                //throw std::runtime_error("USBCamera::set_value::Not implemented");
                return true;
        }

        bool USBCamera::open(size_t width, size_t height)
        {
                bool success = false;
                
                r_info("USBCamera::open: %s, %ux%u", _device.c_str(),
                       width, height);

                _camera = new_camera(_device.c_str(), width, height);
                if (_camera != nullptr) {
                        start_capture_thread();
                        success = true;
                } else {
                        r_err("USBCamera::open: failed to create the camera");
                }
                
                return success;
        }

        void USBCamera::start_capture_thread()
        {
                _thread = std::thread(&USBCamera::run, this);
        }


        void USBCamera::run()
        {
                while (!_done) {
                        grab_from_camera();
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                }
        }

        void USBCamera::grab_from_camera()
        {
                SynchonizedCodeBlock synchonized(_mutex);
                
                if (camera_capture(_camera) == 0) {
                        uint8_t *rgb = camera_getimagebuffer(_camera);
                        size_t width = camera_width(_camera);
                        size_t height = camera_height(_camera);
                        _image.import(Image::RGB, rgb, width, height);
                        
                } else {
                        r_warn("USBCamera: camera_capture failed");
                }
        }
        
        bool USBCamera::grab(Image &image)
        {
                SynchonizedCodeBlock synchonized(_mutex);
                r_info("USBCamera: grab");
                image = _image;
                return true;
        }
        
        rcom::MemBuffer& USBCamera::grab_jpeg()
        {
                SynchonizedCodeBlock synchonized(_mutex);
                
                r_info("USBCamera: grab");
                
                std::vector<uint8_t> buffer;
                ImageIO::store_jpg_to_buffer(_image, buffer);
                _buffer.clear();
                _buffer.append(buffer.data(), buffer.size());

                return _buffer;
                
                // r_err("USBCamera::grab_jpeg: Not implemented");
                // throw std::runtime_error("USBCamera::grab_jpeg: Not implemented");
        }
        
        bool USBCamera::power_up()
        {
                return true;
        }
        
        bool USBCamera::power_down()
        {
                return true;
        }
        
        bool USBCamera::stand_by()
        {
                return true;
        }
        
        bool USBCamera::wake_up()
        {
                return true;
        }

        const ICameraSettings& USBCamera::get_settings()
        {
                throw std::runtime_error("USBCamera::get_settings: not implemented");
        }
}
