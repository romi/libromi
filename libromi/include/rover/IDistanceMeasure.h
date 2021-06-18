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
#ifndef __ROMI_I_DISTANCEMEASURE_H
#define __ROMI_I_DISTANCEMEASURE_H

namespace romi {

        /**
           Measures the distance only along the direction of the
           track. To measure the distance perpendicular to the track
           (cross-track error), use the ITrackFollower.

           The distance is measured from a point p1 (the current
           location) to a point p2 located at a given distance in
           front of the rover. When the rover moves from p1 to p2 on a
           straight line, the distance from start will increase from 0
           to the given distance and will continue to increase when
           the rover continuous beyond the end point. The distance to
           the end point will be the given distance at the beginning
           and decrease to become zero when the end point is
           reached. This distance will become negative when the rover
           continuous beyond the end point. So the distance-from-start
           and distance-to-end have an increasing and decrasing slope,
           respectively. Also, distance-from-start is negative before
           the start point and distance-to-end is negative beyond the
           end point.
           
         */
        class IDistanceMeasure
        {
        public:
                virtual ~IDistanceMeasure() = default;
                
                virtual bool set_distance_to_navigate(double distance) = 0;
                virtual double get_distance_to_navigate() = 0;

                virtual bool update_distance_estimate() = 0;
                virtual double get_distance_from_start() = 0;
                virtual double get_distance_to_end() = 0;
        };
}

#endif // __ROMI_I_DISTANCEMEASURE_H
