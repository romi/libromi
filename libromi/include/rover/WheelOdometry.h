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
#ifndef __ROMI_WHEEL_ODOMETRY_H
#define __ROMI_WHEEL_ODOMETRY_H

#include <mutex>
#include "data_provider/ILocationProvider.h"
#include "data_provider/IOrientationProvider.h"
#include "api/IMotorDriver.h"
#include "rover/ITrackFollower.h"
#include "NavigationSettings.h"

namespace romi {
              
        using SynchronizedCodeBlock = std::lock_guard<std::mutex>;
        
        class WheelOdometry : public ILocationProvider,
                              public IOrientationProvider,
                              public ITrackFollower
        {
        protected:
                IMotorDriver& driver_;
                std::mutex mutex_;
                        
                double instantaneous_speed[2];
                double filtered_speed[2];
                double encoder[2];
                double last_timestamp;
        
                // The current location and orientation
                // The displacement, in meters, and the change in orientation
                // (only tracking the change in yaw) relative to the 'current'
                // location;
                double displacement[2];
                double theta;
                
                double wheel_circumference; 
                double wheel_base;
                double encoder_steps;

                bool update_estimate();
                
        public:
                WheelOdometry(NavigationSettings &rover_config,
                              IMotorDriver& driver);
                
                virtual ~WheelOdometry();

                void set_encoders(double left, double right, double timestamp);
                void set_displacement(double x, double y);

                // ILocationProvider
                bool update_orientation_estimate() override;
                v3 get_location() override;
                std::string get_location_string() override;
                
                // IOrientationProvider
                bool update_location_estimate() override;
                double get_orientation() override;

                // ITrackFollower
                void start() override;
                bool update_error_estimate() override;
                double get_cross_track_error() override;
                double get_orientation_error() override;
                
                v3 get_encoders();
                v3 get_speed();
        };
}

#endif // __ROMI_WHEEL_ODOMETRY_H
