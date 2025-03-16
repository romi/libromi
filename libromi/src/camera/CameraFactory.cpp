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
#include <rcom/RcomClient.h>
#include "util/Logger.h"
#include "camera/FakeCamera.h"
#include "camera/FileCamera.h"
#include "camera/ExternalCamera.h"
#include "camera/CameraWithConfig.h"
#include "camera/USBCamera.h"
#include "camera/LibCamera.h"
#include "rpc/RemoteCamera.h"
#include "camera/CameraFactory.h"

namespace romi {

        std::unique_ptr<ICamera> CameraFactory::create(
                rcom::ILinux&,
                std::shared_ptr<rcom::ILog>& rcomlog,
                std::shared_ptr<ICameraInfoIO>& io)
        {
                r_debug("CameraFactory::create");
                
                auto info = io->load();
                
                // Camera
                std::unique_ptr<ICamera> real_camera
                        = make_camera(rcomlog, info->get_settings());
                std::unique_ptr<ICamera> camera
                        = std::make_unique<CameraWithConfig>(io, real_camera);

                return camera;
        }

        std::unique_ptr<ICamera> CameraFactory::make_camera(
                std::shared_ptr<rcom::ILog>& rcomlog,
                ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_camera");
                
                if (settings.type() == "external-camera")
                        return make_external_camera(settings);
                        
                else if (settings.type() == "remote-camera")
                        return make_remote_camera(rcomlog, settings);
                        
                else if (settings.type() == "fake-camera")
                        return make_fake_camera(settings);
                
                else if (settings.type() == "file-camera")
                        return make_file_camera(settings);

                else if (settings.type() == "v4l-camera")
                        return make_usb_camera(settings);

                else if (settings.type() == "libcamera")
                        return make_libcamera(settings);

                else {
                        throw std::runtime_error("Unknown camera type");
                }
        }

        std::unique_ptr<ICamera> CameraFactory::make_fake_camera(ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_fake_camera");
                
                size_t width = (size_t) settings.get_value(ICameraSettings::kWidth);
                size_t height = (size_t) settings.get_value(ICameraSettings::kHeight);
                
                std::unique_ptr<ICamera> camera
                        = std::make_unique<FakeCamera>(width, height);
                return camera;
        }
        
        std::unique_ptr<ICamera> CameraFactory::make_file_camera(ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_file_camera");

                std::string path;
                settings.get_option(ICameraSettings::kPath, path);
                
                std::unique_ptr<ICamera> camera
                        = std::make_unique<FileCamera>(path);
                return camera;
        }

        std::unique_ptr<ICamera> CameraFactory::make_external_camera(ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_external_camera");
                
                std::string executable;
                settings.get_option(ICameraSettings::kPath, executable);
                
                std::unique_ptr<ICamera> camera
                        = std::make_unique<ExternalCamera>(executable);
                return camera;
        }

        std::unique_ptr<ICamera> CameraFactory::make_remote_camera(
                std::shared_ptr<rcom::ILog>& rcomlog,
                ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_remote_camera");
                
                std::string topic;
                settings.get_option(ICameraSettings::kTopic, topic);
                
                auto client = rcom::RcomClient::create(topic, 10.0, rcomlog);
                std::unique_ptr<ICamera> camera
                        = std::make_unique<RemoteCamera>(client);
                return camera;
        }

        std::unique_ptr<ICamera> CameraFactory::make_usb_camera(ICameraSettings& settings)
        {
                r_debug("CameraFactory::make_usb_camera");
                
                std::string device;
                settings.get_option(ICameraSettings::kDevice, device);
                size_t width = (size_t) settings.get_value(ICameraSettings::kWidth);
                size_t height = (size_t) settings.get_value(ICameraSettings::kHeight);
                
                std::unique_ptr<ICamera> camera
                        = std::make_unique<USBCamera>(device, width, height);
                return camera;
        }

        std::unique_ptr<ICamera> CameraFactory::make_libcamera(ICameraSettings&)
        {
                r_debug("CameraFactory::make_libcamera");
                
                std::unique_ptr<ICamera> camera
                        = std::make_unique<LibCamera>();
                return camera;
        }
}
