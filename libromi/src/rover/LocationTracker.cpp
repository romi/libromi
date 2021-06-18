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
#include <math.h>
#include <ClockAccessor.h>
#include "rover/LocationTracker.h"

namespace romi {

        LocationTracker::LocationTracker(ILocationProvider& location_provider,
                                         IOrientationProvider& orientation_provider)
                : location_provider_(location_provider),
                  orientation_provider_(orientation_provider),
                  distance_(0.0),
                  p1_(0.0),
                  p2_(0.0),
                  p0_(0.0),
                  ex_(0.0),
                  ey_(0.0),
                  theta0_(0.0),
                  theta_(0.0),
                  last_time_(0.0),
                  distance_from_start_(0.0),
                  cross_track_error_(0.0)
        {
        }

        bool LocationTracker::update_estimates()
        {
                bool success = (location_provider_.update_location_estimate()
                                && orientation_provider_.update_orientation_estimate());
                return success;
        }

        bool LocationTracker::set_distance_to_navigate(double distance)
        {
                distance_ = distance;

                bool success = update_estimates();

                if (success) {
                        p1_ = location_provider_.get_location();
                        p0_ = p1_;

                        theta0_ = orientation_provider_.get_orientation();
                        theta_ = theta0_;

                        if (distance != 0) {
                                double dx = distance * cos(theta0_);
                                double dy = distance * sin(theta0_);
                                p2_ = v3(p1_.x() + dx, p1_.y() + dy, 0.0);
                                ex_ = v3(dx, dy, 0.0);
                                ey_ = v3(-dy, dx, 0.0);
                                ex_.normalize();
                                ey_.normalize();
                        } else {
                                double dx = cos(theta0_);
                                double dy = sin(theta0_);
                                p2_ = p1_;
                                ex_ = v3(dx, dy, 0.0);
                                ey_ = v3(-dy, dx, 0.0);
                        }
                }

                return success;
        }

        double LocationTracker::get_distance_to_navigate()
        {
                return distance_;
        }

        bool LocationTracker::update_perhaps()
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double now = clock->time();
                bool success = false;
                if (now - last_time_ >= kMinimumUpdateInterval) {
                        success = update();
                        last_time_ = now;
                }
                return success;
        }

        bool LocationTracker::update()
        {
                bool success = update_estimates();
                if (success) {
                        p0_ = location_provider_.get_location();
                        theta_ = orientation_provider_.get_orientation();

                        v3 q = p0_ - p1_;
                        distance_from_start_ = q.dot(ex_);
                        cross_track_error_ = q.dot(ey_);
                }

                return success;
        }

        bool LocationTracker::update_distance_estimate()
        {
                return update_perhaps();
        }

        double LocationTracker::get_distance_from_start()
        {
                return distance_from_start_;
        }

        double LocationTracker::get_distance_to_end()
        {
                return distance_ - distance_from_start_;
        }

        bool LocationTracker::update_error_estimate()
        {
                return update_perhaps();
        }

        double LocationTracker::get_cross_track_error()
        {
                return cross_track_error_;
        }

        double LocationTracker::get_orientation_error()
        {
                return theta_ - theta0_;
        }

}
