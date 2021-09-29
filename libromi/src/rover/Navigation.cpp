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

#include <algorithm>
#include <log.h>
#include <ClockAccessor.h>
#include "rover/WheelOdometry.h"
#include "rover/Navigation.h"
#include "rover/ZeroNavigationController.h"
#include "rover/L1NavigationController.h"
#include "rover/PythonTrackFollower.h"
#include "api/DataLogAccessor.h"


#include <RomiSerialClient.h>
#include <RSerial.h>
#include <oquam/StepperController.h>
#include <oquam/StepperSettings.h>

namespace romi {

        static const double kSlowNavigationSpeed = 0.2;
        static const double kDistanceSlowNavigation = 0.1;

        static const std::string kSpeedName = "navigation-speed";
        static const std::string kDistanceName = "navigation-distance";
        static const std::string kDistanceFromStartName = "navigation-distance-from-start";
        static const std::string kDistanceToEndName = "navigation-distance-to-end";
        static const std::string kCrossTrackErrorName = "navigation-cross-track-error";
        static const std::string kOrientationErrorName = "navigation-orientation-error";

        Navigation::Navigation(NavigationSettings &settings,
                               IDistanceMeasure& distance_measure,
                               ITrackFollower& track_follower,
                               INavigationController& navigation_controller,
                               ISteering& steering,
                               ISession& session)
                : settings_(settings),
                  distance_measure_(distance_measure),
                  track_follower_(track_follower),
                  navigation_controller_(navigation_controller),
                  steering_(steering),
                  session_(session),
                  mutex_(),
                  status_(MOVEAT_CAPABLE),
                  stop_(false)
        {
                moveat(0.0, 0.0);
        }
        
        Navigation::~Navigation()
        {
                moveat(0.0, 0.0);
        }

        bool Navigation::do_move(double distance, double speed)
        {
                bool success = false;

                if (distance == 0.0) {
                        success = true;

                } else if (speed != 0.0
                           && speed >= -1.0
                           && speed <= 1.0
                           && distance >= -50.0 // 50 meters max!
                           && distance <= 50.0) {

                        if (distance * speed >= 0.0) {
                                // All is well, moving forward
                                distance = fabs(distance);
                                speed = fabs(speed);
                        } else {
                                // Moving backwards. Make sur the
                                // distance is positive and the speed
                                // negative.
                                distance = -fabs(distance);
                                speed = -fabs(speed);
                        }

                        if (steering_.stop()) {

                                success = travel(distance, speed);

                        } else {
                                r_err("Navigation::do_move: stop failed");
                        }

                } else {
                        r_err("Navigation::do_move: invalid speed or distance: "
                              "speed=%f, distance=%f", speed, distance);
                }

                // A bit of a hack: in any case, make sure that the
                // rover comes to a standstill.
                steering_.stop();

                return success;
        }

        bool Navigation::travel(double distance, double speed)
        {
                bool success = false;

                try {
                        double timeout = compute_timeout(distance, speed);
                        success = try_travel(speed, distance, timeout);

                } catch (const std::runtime_error &re) {
                        r_err("Navigation::travel: %s", re.what());
                }

                stop();

                return success;
        }
        
        // TODO: Spin off a seperate thread so the main event loop can continue?
        bool Navigation::try_travel(double speed, double distance, double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                v3 start_location;
                bool success = false;
                double now;

                /*
                  Estimate the distance needed to slow down from
                  travel speed to stand-still.

                  v1 = v0 - a.dt (v0=speed; v1=0)
                  => 0 = v - a.dt => dt = v/a
                
                  x1 = x0 + v0.dt - 0.5 a.dt² (x0=0, v0=speed, dt=v/a)
                  => x = 0.5 v²/a
                */
                double slowdown_distance = (0.5 * speed * speed
                                            / settings_.maximum_acceleration);
                r_debug("Navigation: slowdown_distance %f", slowdown_distance);
                if (slowdown_distance < kDistanceSlowNavigation)
                        slowdown_distance = kDistanceSlowNavigation;

                stop_ = false;
                
                if (!distance_measure_.set_distance_to_navigate(distance)) {
                        r_err("Navigation::travel: pose estimation failed");
                        return false;
                }

                now = clock->time();
                log_data(now, kSpeedName, speed);
                log_data(now, kDistanceName, distance);

                if (!track_follower_.start_line()) {
                        r_err("Navigation::travel: track_follower_.start_line failed");
                        return false; // TODO
                }
                
                while (!stop_) {

                        // TODO: HANDLE USER INPUT AND HANDLE STOP
                        // REQUESTED USER-REQUESTED

                        // How far off-track is the rover?
                        if (!track_follower_.update_error_estimate()) {
                                r_err("Navigation::travel: track error estimation failed");
                                break;
                        }
                        
                        double cross_track_error = track_follower_.get_cross_track_error();
                        double orientation_error = track_follower_.get_orientation_error();
                        
                        SteeringData direction = navigation_controller_.estimate_steering(cross_track_error, orientation_error);
                        steering_.drive(speed, direction);
                        
                        // Where are we now?
                        if (!distance_measure_.update_distance_estimate()) {
                                r_err("Navigation::travel: distance estimation failed");
                                break;
                        }
                        double distance_to_end = distance_measure_.get_distance_to_end();

                        // If the rover is close to the end, force a slow-down.
                        if (distance_to_end <= slowdown_distance) {
                                if (speed > kSlowNavigationSpeed) {
                                        speed = kSlowNavigationSpeed;
                                } else if (speed < -kSlowNavigationSpeed){
                                        speed = -kSlowNavigationSpeed;
                                }
                        }

                        // The rover has arrived
                        if (distance_to_end <= 0.0) {
                                success = true;
                                break;
                        }

                        now = clock->time();
                        if (now - start_time >= timeout) {
                                r_err("Navigation::travel: time out (%f s)", timeout);
                                break;
                        }

                        log_data(now, kCrossTrackErrorName, cross_track_error);
                        log_data(now, kOrientationErrorName, orientation_error);
                        log_data(now, kDistanceFromStartName, distance_measure_.get_distance_from_start());
                        log_data(now, kDistanceToEndName, distance_measure_.get_distance_to_end());
                        
                        clock->sleep(0.020);
                }

                return success;
        }

        double Navigation::compute_timeout(double distance, double absolute_speed)
        {
                double timeout = 0.0;
                if (absolute_speed != 0.0) {
                        double time = fabs(distance) / fabs(absolute_speed);
                        timeout = 7.0 + 2.0 * time;

                        r_debug("Navigation::compute_timeout: v=%f, v_max=%f, "
                                "dist=%f, dt=%f, timeout=%f", absolute_speed,
                                settings_.maximum_speed, distance, time, timeout);
                }
                return timeout;
        }

        bool Navigation::moveat(double left, double right)
        {
                SynchronizedCodeBlock sync(mutex_);
                bool success = false;
                double speed = (left + right) / 2.0;
                if (right == left) {
                        success = steering_.forward(speed);
                } else {
                        double delta = right - left;
                        double radius = settings_.wheeltrack / (2.0 * delta);
                        success = steering_.turn(speed, radius);
                }
                return success;
        }

        bool Navigation::move(double distance, double speed)
        {
                bool success = false;
                {
                        SynchronizedCodeBlock sync(mutex_);
                        if (status_ == MOVEAT_CAPABLE) {
                                status_ = MOVING;
                        } else {
                                r_warn("Navigation::move: already moving");
                        }
                }

                if (status_ == MOVING)
                        success = do_move(distance, speed);

                status_ = MOVEAT_CAPABLE;

                return success;
        }

        bool Navigation::stop()
        {
                // This flag should assure that we break out
                // the travel loop.
                stop_ = true;
                bool success = steering_.stop();
                status_ = MOVEAT_CAPABLE;
                return success;
        }

        bool Navigation::pause_activity()
        {
                r_warn("*** Navigation::pause_activity NOT YET IMPLEMENTED");
                stop();
                return true;
        }

        bool Navigation::continue_activity()
        {
                r_warn("*** Navigation::continue_activity NOT YET IMPLEMENTED");
                return true;
        }

        bool Navigation::reset_activity()
        {
                stop();
                return true;
        }

        bool Navigation::enable_controller()
        {
                // return driver_.enable_controller();
                return true;
        }

        bool Navigation::disable_controller()
        {
                // return driver_.disable_controller();
                return true;
        }

        bool Navigation::initialise() {
                return steering_.initialise();
        }
}
