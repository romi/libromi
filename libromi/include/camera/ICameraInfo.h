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

#ifndef __ROMI_ICAMERAINFO_H
#define __ROMI_ICAMERAINFO_H

#include <memory>
#include <utility>
#include <string>
#include "camera/ICameraIntrinsics.h"
#include "camera/ICameraSettings.h"
#include "camera/ICameraDistortion.h"

namespace romi {
        
        class ICameraInfo
        {
        public:
                static constexpr const char *kPiCameraHQ1 = "pi-camera-hq-v10";
                static constexpr const char *kExternalCamera = "external-camera";

                virtual ~ICameraInfo() = default;

                virtual std::string& get_id() = 0;
                virtual std::string& get_type() = 0;
                
                virtual std::string& get_name() = 0;
                virtual void set_name(const std::string& value) = 0;
                
                virtual std::string& get_lens() = 0;
                virtual void set_lens(const std::string& value) = 0;
                
                virtual std::pair<double,double>& get_sensor_resolution() = 0;
                virtual void set_sensor_resolution(double rx, double ry) = 0;
                
                virtual std::pair<double,double>& get_sensor_dimensions() = 0;
                virtual void set_sensor_dimensions(double dx, double dy) = 0;

                virtual std::string& get_calibration_date() = 0;
                virtual void set_calibration_date(const std::string& value) = 0;
                
                virtual std::string& get_calibration_person() = 0;
                virtual void set_calibration_person(const std::string& value) = 0;
                
                virtual std::string& get_calibration_method() = 0;
                virtual void set_calibration_method(const std::string& value) = 0;
                
                virtual ICameraIntrinsics& get_intrinsics() = 0;
                virtual ICameraSettings& get_settings() = 0;
                virtual ICameraDistortion& get_distortion() = 0;
        };
}

#endif // __ROMI_ICAMERAINFO_H
