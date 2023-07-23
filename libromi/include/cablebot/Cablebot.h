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
#ifndef __ROMI_CABLEBOT_H
#define __ROMI_CABLEBOT_H

#include <memory>
#include <IRomiSerialClient.h>
#include <rcom/ILinux.h>
#include "hal/ImagingDevice.h"
#include "camera/ICameraInfoIO.h"
#include "camera/ICameraInfo.h"
#include "api/IGimbal.h"

namespace romi {
        
        class Cablebot
        {
        public:

                enum CameraMode { kVideoMode, kStillMode };

                static const uint32_t kDefaultBitrate = 25000000;
                static const uint32_t kHighBitrate = 25000000;
                static const uint32_t kAverageBitrate = 17000000;
                static const uint32_t kLowBitrate = 8000000;

                static const constexpr char *kSerialBase = "/dev/serial0";
                static const constexpr char *kSerialGimbal = "/dev/serial1";
                static const constexpr char *kGimbalI2CDevice = "/dev/i2c-1";
                static const int kGimbalI2CAddress = 11;
                
                static std::unique_ptr<ImagingDevice>
                create(rcom::ILinux& linux,
                       std::shared_ptr<ICameraInfoIO>& io);
                
        protected:
                static std::shared_ptr<ICamera> make_camera(ICameraSettings& settings);
                static std::shared_ptr<ICamera> make_pi_camera(ICameraSettings& settings);
                static std::shared_ptr<ICamera> make_fake_camera(ICameraSettings& settings);
                static std::shared_ptr<ICamera> make_external_camera(ICameraSettings& settings);

                static std::unique_ptr<IGimbal> make_gimbal(rcom::ILinux& linux);
                static std::unique_ptr<IGimbal> make_i2c_gimbal(rcom::ILinux& linux);
                static std::unique_ptr<IGimbal> make_fake_gimbal();
                
                static std::unique_ptr<romiserial::IRomiSerialClient> connect_base();
                static std::unique_ptr<romiserial::IRomiSerialClient> connect_real_base();
                static std::unique_ptr<romiserial::IRomiSerialClient> connect_fake_base();

                static CameraMode get_mode(ICameraSettings& settings);
                static void get_resolution(ICameraSettings& settings,
                                           size_t& width, size_t& height);
                static int32_t get_framerate(ICameraSettings& settings);
                static uint32_t get_bitrate(ICameraSettings& settings);
        };
}

#endif // __ROMI_CABLEBOT_H
