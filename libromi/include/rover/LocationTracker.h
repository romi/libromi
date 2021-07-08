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
#ifndef __ROMI_LOCATION_TRACKER_H
#define __ROMI_LOCATION_TRACKER_H

#include "data_provider/ILocationProvider.h"
#include "data_provider/IOrientationProvider.h"
#include "rover/IDistanceMeasure.h"
#include "rover/ITrackFollower.h"

namespace romi {
        
        class LocationTracker : public IDistanceMeasure, public ITrackFollower
        {
        public:
                static constexpr double kMinimumUpdateInterval = 0.010;
                
        protected:
                ILocationProvider& location_provider_;
                IOrientationProvider& orientation_provider_;

                double distance_;
                v3 p1_;
                v3 p2_;
                v3 p0_;
                v3 ex_;
                v3 ey_;
                double theta0_;
                double theta_;
                double last_time_;
                double distance_from_start_;
                double cross_track_error_;
                
                bool update_estimates();
                bool update_perhaps();
                bool update();

        public:
                LocationTracker(ILocationProvider& location_provider,
                                IOrientationProvider& orientation_provider);
                ~LocationTracker() override = default;

                // IDistanceMeasure
                bool set_distance_to_navigate(double distance) override;
                double get_distance_to_navigate() override;
                
                bool update_distance_estimate() override;
                double get_distance_from_start() override;
                double get_distance_to_end() override;

                // ITrackFollower
                bool update_error_estimate() override;
                double get_cross_track_error() override;
                double get_orientation_error() override;
                
        };
}

#endif // __ROMI_LOCATION_TRACKER_H
