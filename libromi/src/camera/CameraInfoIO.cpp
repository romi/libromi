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
#include <iostream>
#include "util/Logger.h"
#include "camera/CameraInfoIO.h"
#include "camera/CameraInfo.h"
#include "camera/CameraIntrinsics.h"
#include "camera/CameraSettings.h"
#include "camera/SimpleRadialDistortion.h"


namespace romi {
        
        CameraInfoIO::CameraInfoIO(std::shared_ptr<IConfigManager>& config,
                                   const std::string& section)
                : config_(config),
                  section_(section)
        {
        }

        nlohmann::json CameraInfoIO::get()
        {
                return config_->get_section(section_);
        }
        
        std::unique_ptr<ICameraInfo> CameraInfoIO::load()
        {
                nlohmann::json json = get();

                std::cout << json << std::endl;
                
                std::string camera_type = json[kCameraType];

                r_debug("CameraInfoIO::load: type: %s", camera_type.c_str());
                
                std::unique_ptr<ICameraIntrinsics> intrinsics
                        = load_intrinsics(json[kIntrinsics]);
                        
                std::unique_ptr<ICameraSettings> settings
                        = load_settings(camera_type, json[camera_type]);
                
                std::unique_ptr<ICameraDistortion> distortion
                        = load_distortion(json[kDistortion]);

                return load_info(json, intrinsics, settings, distortion);
        }
        
        std::unique_ptr<ICameraIntrinsics>
        CameraInfoIO::load_intrinsics(nlohmann::json& json)
        {
                double fx = json[kFx];
                double fy = json[kFy];
                double cx = json[kCx];
                double cy = json[kCy];
                std::unique_ptr<ICameraIntrinsics> result
                        = std::make_unique<CameraIntrinsics>(fx, fy, cx, cy);
                return result;
        }
        
        std::unique_ptr<ICameraSettings>
        CameraInfoIO::load_settings(const std::string& camera_type,
                                    nlohmann::json& json_settings)
        {
                // if (camera_type != ICameraInfo::kPiCameraHQ1) {
                //         r_err("CameraInfoIO::load_settings: unhandled camera type: %s",
                //               camera_type.c_str());
                //         throw std::runtime_error("CameraInfoIO::load_settings");
                // }
                
                std::unique_ptr<ICameraSettings> result
                        = std::make_unique<CameraSettings>(camera_type, json_settings);
                return result;
        }
        
        std::unique_ptr<ICameraDistortion>
        CameraInfoIO::load_distortion(nlohmann::json& json)
        {
                std::string type = json[kDistortionType];
                if (type != ICameraDistortion::kSimpleRadialDistortion) {
                        r_err("CameraInfoIO::load_distortion: unhandled distortion: %s",
                              type.c_str());
                        throw std::runtime_error("CameraInfoIO::load_distortion");
                }
                
                std::vector<double> values =
                        json[kDistortionValues].get<std::vector<double>>();
        
                std::unique_ptr<ICameraDistortion> result
                        = std::make_unique<SimpleRadialDistortion>(values);
                
                return result;
        }

        std::unique_ptr<ICameraInfo>
        CameraInfoIO::load_info(nlohmann::json& json,
                                std::unique_ptr<ICameraIntrinsics>& intrinsics,
                                std::unique_ptr<ICameraSettings>& settings,
                                std::unique_ptr<ICameraDistortion>& distortion)
        {
                std::string id = json[kCameraID];
                std::string type = json[kCameraType];
                std::unique_ptr<ICameraInfo> result
                        = std::make_unique<CameraInfo>(id, type, intrinsics,
                                                       settings, distortion);

                std::string name = json.value(kCameraName, "unspecified");
                std::string lens = json.value(kCameraLens, "unspecified");
                result->set_name(name);
                result->set_lens(lens);

                if (json.contains(kSensor)) {
                        double rx = json[kSensor][kSensorResolution][0];
                        double ry = json[kSensor][kSensorResolution][1];
                        double dx = json[kSensor][kSensorDimensions][0];
                        double dy = json[kSensor][kSensorDimensions][1];
                        result->set_sensor_resolution(rx, ry);
                        result->set_sensor_dimensions(dx, dy);
                }
                
                if (json.contains(kCalibration)) {
                        std::string date = json[kCalibration][kCalibrationDate];
                        std::string person = json[kCalibration][kCalibrationPerson];
                        std::string method = json[kCalibration][kCalibrationMethod];
                        result->set_calibration_date(date);
                        result->set_calibration_person(person);
                        result->set_calibration_method(method);
                }
                
                return result;
        }

        void CameraInfoIO::store(ICameraInfo& info)
        {
                nlohmann::json json = to_json(info);
                config_->set_section(section_, json);
        }

        nlohmann::json CameraInfoIO::to_json(ICameraInfo& info)
        {
                // Only the camera settings can be updated
                // dynamically. First, we recover the original JSON,
                // then we overwrite the camera settings. This makes
                // sure that we copy any object in the 'camera'
                // section that we don't handle.
                nlohmann::json json = config_->get_section(section_);
                std::string camera_type = json[kCameraType];
                ICameraSettings& settings = info.get_settings();
                json[camera_type] = settings.get_all();
                return json;
        }

        /*
        nlohmann::json CameraInfoIO::to_json(ICameraInfo& info)
        {
                ICameraIntrinsics& intrinsics = info.get_intrinsics();
                double fx, fy, cx, cy;
                intrinsics.get_focal_length(fx, fy);
                intrinsics.get_central_point(cx, cy);

                ICameraDistortion& distortion = info.get_distortion();
                std::vector<double> distortion_values;
                distortion.get(distortion_values);

                ICameraSettings& settings = info.get_settings();

                nlohmann::json json{
                        {CameraInfoIO::kCameraType, info.get_type()},
                        {CameraInfoIO::kCameraID, info.get_id()},
                        {CameraInfoIO::kCameraName, info.get_name()},
                        {CameraInfoIO::kCameraLens, info.get_lens()},
                        {CameraInfoIO::kSensor,
                                {
                                        {CameraInfoIO::kSensorResolution,
                                                {info.get_sensor_resolution().first,
                                                                info.get_sensor_resolution().second}},
                                        {CameraInfoIO::kSensorDimensions,
                                                {info.get_sensor_dimensions().first,
                                                                info.get_sensor_dimensions().second}}
                                }},
                        {CameraInfoIO::kCalibration,
                                {
                                        {CameraInfoIO::kCalibrationDate,
                                                        info.get_calibration_date()},
                                        {CameraInfoIO::kCalibrationPerson,
                                                        info.get_calibration_person()},
                                        {CameraInfoIO::kCalibrationMethod,
                                                        info.get_calibration_method()}
                                }},
                        {CameraInfoIO::kIntrinsics,
                                {
                                        {CameraInfoIO::kFx, fx},
                                        {CameraInfoIO::kFy, fy},
                                        {CameraInfoIO::kCx, cx},
                                        {CameraInfoIO::kCy, cy}
                                }},
                        {CameraInfoIO::kSettings, settings.get_all()},
                        {CameraInfoIO::kDistortion,
                                {
                                        {CameraInfoIO::kDistortionType,
                                                        distortion.get_type()},
                                        {CameraInfoIO::kDistortionValues,
                                                        distortion_values}
                                }}
                };
                return json;
        }
        */
}
