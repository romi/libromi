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

namespace romi {

        struct NavRecording
        {
                double time_;
                double location_x_;
                double location_y_;
                double distance_;
                double cross_track_error_;
                double orientation_error_;
                double correction_;
                double left_speed_;
                double right_speed_;

                NavRecording(double time,
                             double location_x,
                             double location_y,
                             double distance,
                             double cross_track_error,
                             double orientation_error,
                             double correction,
                             double left_speed,
                             double right_speed)
                        : time_(time),
                          location_x_(location_x),
                          location_y_(location_y),
                          distance_(distance),
                          cross_track_error_(cross_track_error),
                          orientation_error_(orientation_error),
                          correction_(correction),
                          left_speed_(left_speed),
                          right_speed_(right_speed) {
                };
        };

        Navigation::Navigation(IMotorDriver &driver,
                               NavigationSettings &settings,
                               ITrackFollower& track_follower,
                               ISession& session)
                : driver_(driver),
                  settings_(settings),
                  track_follower_(track_follower),
                  session_(session),
                  mutex_(),
                  status_(MOVEAT_CAPABLE),
                  stop_(false),
                  update_thread_(),
                  max_acceleration_(0.0),
                  left_target_(0.0),
                  right_target_(0.0),
                  left_speed_(0.0),
                  right_speed_(0.0),
                  quitting_(false)
        {
                max_acceleration_  = 0.1; 
                update_thread_ = std::make_unique<std::thread>(
                        [this]() {
                                update_speeds();
                        });
        }

        Navigation::~Navigation()
        {
                quitting_ = true;
                if (update_thread_) {
                        update_thread_->join();
                        update_thread_ = nullptr;
                }
        }

        void Navigation::update_speeds()
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double last_time = clock->time();

                while (!quitting_) {
                        double now = clock->time();
                        double dt = now - last_time;
                        double left = compute_next_speed(left_speed_, left_target_, dt);
                        double right = compute_next_speed(right_speed_, right_target_, dt);
                        
                        if (left != left_speed_ || right != right_speed_) {
                                left_speed_ = left;
                                right_speed_ = right;
                                send_moveat(left_speed_, right_speed_);
                                r_debug("Navigation: speed now (%.2f, %.2f)",
                                        left_speed_, right_speed_);
                        }

                        clock->sleep(0.040);
                }
        }
        
        double Navigation::compute_next_speed(double current_speed,
                                              double target_speed,
                                              double dt)
        {
                double new_speed = current_speed;
                if (current_speed < target_speed) {
                        new_speed += max_acceleration_ * dt;
                        if (new_speed > target_speed)
                                new_speed = target_speed;
                } else if (current_speed > target_speed) {
                        new_speed -= max_acceleration_ * dt;
                        if (new_speed < target_speed)
                                new_speed = target_speed;
                }
                return new_speed;
        }
        
        bool Navigation::set_speed_targets(double left, double right)
        {
                left_target_ = left;
                right_target_ = right;
                r_debug("Navigation: Speed target now (%.2f, %.2f)", left, right);
                return true;
                //return send_moveat(left, right);
        }

        bool Navigation::send_moveat(double left, double right)
        {
                left = std::clamp(left, -1.0, 1.0);
                right = std::clamp(right, -1.0, 1.0);
                return driver_.moveat(left, right);
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
                                distance = fabs(distance);
                                speed = -fabs(speed);
                        }
                        
                        if (driver_.stop()) {
                                
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
                driver_.stop();

                return success;
        }
        
        bool Navigation::travel(double distance, double speed)
        {
                bool success = false;

                try {
                        WheelOdometry odometry(settings_, driver_);
                        //odometry.set_displacement(0, 0.1); // DEBUG
                        
                        ZeroNavigationController navigation_controller;
                        //L1NavigationController navigation_controller(settings_.wheel_base,
                        //                                             2.0);
                        success = try_travel(odometry,
                                             navigation_controller,
                                             speed, distance,
                                             compute_timeout(distance, speed));
                        
                } catch (const std::runtime_error& re) {
                        r_err("Navigation::travel: %s", re.what());
                }
                
                stop();
                
                return success;
        }

        double Navigation::compute_timeout(double distance, double relative_speed)
        {
                double timeout = 0.0;
                if (relative_speed != 0.0) {
                        double speed = settings_.maximum_speed * relative_speed;
                        double time = distance / fabs(speed);
                        timeout = 7.0 + 2.0 * time;

                        r_debug("Navigation::compute_timeout: v_rel=%f, v_max=%f, "
                                "v=%f, dist=%f, dt=%f, timeout=%f", relative_speed,
                                settings_.maximum_speed, speed, distance, time, timeout);
                }
                return timeout;
        }

        // TODO: Spin off a seperate thread so the main event loop can continue?
        bool Navigation::try_travel(ILocationProvider& location_provider,
                                    INavigationController& navigation_controller,
                                    double speed,
                                    double distance,
                                    double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                v3 start_location;
                std::vector<NavRecording> recording;
                double left_speed = speed;
                double right_speed = speed;
                double distance_travelled;
                bool success = false;
                
                stop_ = false;
                if (!location_provider.update_location_estimate()) {
                        r_err("Navigation::travel: pose estimation failed");
                        return false;
                }
                start_location = location_provider.get_location();
                track_follower_.start();
                
                while (!stop_) {

                        // Keep going
                        if (!set_speed_targets(left_speed, right_speed)) {
                                r_err("Navigation::travel: moveat failed");
                                break;
                        }

                        // TODO: HANDLE USER INPUT AND HANDLE STOP
                        // REQUESTED USER-REQUESTED

                        // Where are we now?
                        if (!location_provider.update_location_estimate()) {
                                r_err("Navigation::travel: pose estimation failed");
                                break;
                        }
                        v3 location = location_provider.get_location();

                        // How far off-track is the rover?
                        track_follower_.update_error_estimate();
                        double cross_track_error = track_follower_.get_cross_track_error();
                        double orientation_error = track_follower_.get_orientation_error();

                        // By how much should the wheel speeds be
                        // adapted to get the rover back on the track?
                        double correction = navigation_controller.estimate_correction(
                                cross_track_error, orientation_error); 
                        left_speed = speed * (1.0 + correction);
                        right_speed = speed * (1.0 - correction);

                        // How much has the rover travelled so far?
                        distance_travelled = (location - start_location).norm(); // FIXME!!!

                        // If the rover is close to the end, force a slow-down.
                        if (distance_travelled >= distance - 0.1) {
                                if (speed > 0.0) {
                                        left_speed = 0.1;
                                        right_speed = 0.1;
                                } else {
                                        left_speed = -0.1;
                                        right_speed = -0.1;
                                }
                        }

                        // The rover has arrived
                        if (distance_travelled >= distance) {
                                success = true;
                                break;
                        }

                        double now = clock->time();
                        if (now - start_time >= timeout) {
                                r_err("Navigation::travel: time out (%f s)", timeout);
                                break;
                        }

                        r_debug("distance travelled %f", distance_travelled);

                        recording.emplace_back(now - start_time,
                                               location.x(),
                                               location.y(),
                                               distance_travelled,
                                               cross_track_error,
                                               orientation_error,
                                               correction,
                                               left_speed,
                                               right_speed);
                        
                        //clock->sleep(0.100);
                }

                set_speed_targets(0.0, 0.0);
                left_speed = 0.0;
                right_speed = 0.0;

                FILE* fp = fopen("/tmp/nav.csv", "w");
                if (fp) {
                        fprintf(fp,
                                "# time\tx\ty\tdistance\tcross-track error\torientation\t"
                                "correction\tleft speed\tright speed\n");
                        for (size_t i = 0; i < recording.size(); i++) {
                                fprintf(fp,
                                        "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t"
                                        "%.6f\t%.6f\t%.6f\t%.6f\n",
                                        recording[i].time_,
                                        recording[i].location_x_,
                                        recording[i].location_y_,
                                        recording[i].distance_,
                                        recording[i].cross_track_error_,
                                        recording[i].orientation_error_,
                                        recording[i].correction_,
                                        recording[i].left_speed_,
                                        recording[i].right_speed_);


                                printf("%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t"
                                       "%.6f\t%.6f\t%.6f\t%.6f\n",
                                       recording[i].time_,
                                       recording[i].location_x_,
                                       recording[i].location_y_,
                                       recording[i].distance_,
                                       recording[i].cross_track_error_,
                                       recording[i].orientation_error_,
                                       recording[i].correction_,
                                       recording[i].left_speed_,
                                       recording[i].right_speed_);
                        }
                        fclose(fp);
                }

                return success;
        }
        
        bool Navigation::moveat(double left, double right)
        {
                SynchronizedCodeBlock sync(mutex_);
                bool success = false;
                if (status_ == MOVEAT_CAPABLE)
                        success = set_speed_targets(left, right);
                else
                        r_warn("Navigation::moveat: still moving");
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
                bool success = driver_.stop();
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
}
