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

#ifndef __ROMI_CAMERAFACTORY_H
#define __ROMI_CAMERAFACTORY_H

#include <memory>
#include <rcom/ILinux.h>
#include <rcom/ILog.h>
#include "api/ICamera.h"
#include "camera/ICameraInfo.h"
#include "camera/ICameraInfoIO.h"

namespace romi {
        
        class CameraFactory
        {
        protected:
                static std::unique_ptr<ICamera> make_camera(
                        std::shared_ptr<rcom::ILog>& rcomlog,
                        ICameraSettings& settings);
                static std::unique_ptr<ICamera> make_fake_camera(
                        ICameraSettings& settings);
                static std::unique_ptr<ICamera> make_file_camera(
                        ICameraSettings& settings);
                static std::unique_ptr<ICamera> make_external_camera(
                        ICameraSettings& settings);
                static std::unique_ptr<ICamera> make_remote_camera(
                        std::shared_ptr<rcom::ILog>& rcomlog,
                        ICameraSettings& settings);
                static std::unique_ptr<ICamera> make_usb_camera(
                        ICameraSettings& settings);
                
        public:
                virtual ~CameraFactory() = default;

                static std::unique_ptr<ICamera> create(
                        rcom::ILinux& linux,
                        std::shared_ptr<rcom::ILog>& rcomlog,
                        std::shared_ptr<ICameraInfoIO>& io);
                
        };
}

#endif // __ROMI_CAMERAFACTORY_H
