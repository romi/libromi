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

#include <cfloat>
#include <stdexcept>
#include "util/Logger.h"
#include "oquam/StepperSettings.h"

// TBD: REFACTOR - remove raw pointers and arrays. Replace with vectors.
namespace romi {
        
        StepperSettings::StepperSettings(nlohmann::json& json)
        {
                try {
                        parse_steps_per_revolution(json);
                        parse_microsteps(json);
                        parse_gears_ratio(json);
                        parse_maximum_rpm(json);
                        parse_displacement_per_revolution(json);
                        parse_maximum_acceleration(json);
                        compute_maximum_speed();
                        compute_steps_per_meter();
                        
                } catch (nlohmann::json::exception& je) {
                        r_err("StepperSettings: Failed to parse the JSON settings");
                        throw std::runtime_error("Invalid settings");;
                }
        }
        
        void StepperSettings::parse_steps_per_revolution(nlohmann::json& json)
        {
                nlohmann::json array = json["steps-per-revolution"];
                parse_array(array, steps_per_revolution);
        }
        
        void StepperSettings::parse_microsteps(nlohmann::json& json)
        {
                nlohmann::json array = json["microsteps"];
                parse_array(array, microsteps);
        }
        
        void StepperSettings::parse_gears_ratio(nlohmann::json& json)
        {
                nlohmann::json array = json["gears-ratio"];
                parse_array(array, gears_ratio);
        }
        
        void StepperSettings::parse_maximum_rpm(nlohmann::json& json)
        {
                nlohmann::json array = json["maximum-rpm"];
                parse_array(array, maximum_rpm);
        }
        
        void StepperSettings::parse_displacement_per_revolution(nlohmann::json& json)
        {
                nlohmann::json array = json["displacement-per-revolution"];
                parse_array(array, displacement_per_revolution);
        }
        
        void StepperSettings::parse_maximum_acceleration(nlohmann::json& json)
        {
                nlohmann::json array = json["maximum-acceleration"];
                parse_array(array, maximum_acceleration);
        }
        
        void StepperSettings::parse_array(nlohmann::json& array, double *values)
        {
                for (size_t i = 0; i < 3; i++) {
                        values[i] = array[i];
                }
        }

        void StepperSettings::compute_maximum_speed()
        {
                for (int i = 0; i < 3; i++) {
                        maximum_speed[i] = (fabs(displacement_per_revolution[i])
                                            * maximum_rpm[i] / 60.0
                                            / gears_ratio[i]);
                }
        }

        void StepperSettings::compute_steps_per_meter()
        {
                for (int i = 0; i < 3; i++) {
                        if (displacement_per_revolution[i] > 0.0) {
                                steps_per_meter[i] = ((gears_ratio[i]
                                                       * microsteps[i]
                                                       * steps_per_revolution[i])
                                                      / displacement_per_revolution[i]);
                        } else {
                                steps_per_meter[i] = 0;
                        }
                }
        }

        double StepperSettings::compute_minimum_duration(double steps)
        {
                double minumim_duration = DBL_MAX;
                for (int i = 0; i < 3; i++) {
                        double duration = compute_duration(steps, i);
                        if (duration < minumim_duration)
                                minumim_duration = duration;
                }
                return minumim_duration;
        }

        double StepperSettings::compute_duration(double steps, int i)
        {
                double max_revolutions_per_sec = maximum_rpm[i] / 60.0;
                double max_steps_per_sec = (max_revolutions_per_sec
                                            * steps_per_revolution[i]
                                            * microsteps[i]);
                double duration = steps / max_steps_per_sec;
                return duration;
        }
}
