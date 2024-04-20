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

#ifndef __ROMI_CAMERAINFOIO_H
#define __ROMI_CAMERAINFOIO_H

#include "api/IConfigManager.h"
#include "camera/ICameraInfoIO.h"

namespace romi {
        
        class CameraInfoIO : public ICameraInfoIO
        {
        protected:
                std::shared_ptr<IConfigManager> config_;
                const std::string section_;
                
        public:
                static constexpr const char *kCameraType = "type";
                static constexpr const char *kCameraID = "id";
                static constexpr const char *kCameraName = "name";
                static constexpr const char *kCameraLens = "lens";
                static constexpr const char *kSensor = "sensor";
                static constexpr const char *kSensorResolution = "resolution";
                static constexpr const char *kSensorDimensions = "dimensions";
                static constexpr const char *kCalibration = "calibration";
                static constexpr const char *kCalibrationDate = "date";
                static constexpr const char *kCalibrationPerson = "person";
                static constexpr const char *kCalibrationMethod = "method";
                static constexpr const char *kIntrinsics = "intrinsics";
                static constexpr const char *kSettings = "settings";
                static constexpr const char *kDistortion = "distortion";
                static constexpr const char *kFx = "fx";
                static constexpr const char *kFy = "fy"; 
                static constexpr const char *kCx = "cx";
                static constexpr const char *kCy = "cy";
                static constexpr const char *kDistortionType = "type";
                static constexpr const char *kDistortionValues = "values";
                
                CameraInfoIO(std::shared_ptr<IConfigManager>& config,
                             const std::string& section);
                ~CameraInfoIO() override = default;

                std::unique_ptr<ICameraInfo> load() override;
                void store(ICameraInfo& info) override;

                nlohmann::json get() override;
                
                nlohmann::json to_json(ICameraInfo& info);
                
        protected:
                std::unique_ptr<ICameraInfo>
                load_info(nlohmann::json& json,
                          std::unique_ptr<ICameraIntrinsics>& intrinsics,
                          std::unique_ptr<ICameraSettings>& settings,
                          std::unique_ptr<ICameraDistortion>& distortion);
                
                std::unique_ptr<ICameraIntrinsics> load_intrinsics(nlohmann::json& json);
                std::unique_ptr<ICameraSettings> load_settings(const std::string& type,
                                                               nlohmann::json& json);
                std::unique_ptr<ICameraDistortion> load_distortion(nlohmann::json& json);
        };
}

#endif // __ROMI_CAMERAINFOIO_H
