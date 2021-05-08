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

#include <log.h>
#include <ClockAccessor.h>
#include "rover/Navigation.h"

namespace romi {

        double Navigation::compute_timeout(double distance, double speed)
        {
                double timeout = 0.0;
                if (speed != 0.0) {
                        double relative_speed = _settings.maximum_speed * speed;
                        double time = distance / fabs(relative_speed);
                        timeout = 1.5 * time;
                }
                return timeout;
        }

        struct NavRecording
        {
                double time_;
                double left_encoder_;
                double right_encoder_;
                double position_x_;
                double position_y_;
                double distance_;

                NavRecording(double t, double left, double right, double x, double y, double d)
                        : time_(t),
                          left_encoder_(left),
                          right_encoder_(right),
                          position_x_(x),
                          position_y_(y),
                          distance_(d) {
                };
        };
        
        // TODO: Spin off a seperate thread so the main event loop can continue?
        bool Navigation::wait_travel(WheelOdometry &odometry, double distance,
                                     double timeout)
        {
                bool success = false;
                double left, right, timestamp;
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                std::vector<NavRecording> recording;
                
                _stop = false;
                
                while (!_stop) {

                        // TODO: HANDLE USER INPUT AND HANDLE STOP REQUESTED USER-REQUESTED
                        
                        _driver.get_encoder_values(left, right, timestamp);
                        odometry.set_encoders(left, right, timestamp);

                        // r_debug("get_encoder_values: %f %f", left, right);

                        double x, y;
                        odometry.get_location(x, y);
                                
                        double distance_travelled = sqrt(x * x + y * y);

                        // r_debug("distance_travelled: %f, distance: %f", distance_travelled, distance);

                        if (distance_travelled >= distance) {
                                _driver.moveat(0, 0);
                                clock->sleep(0.100); // FIXME
                                success = true;
                                _stop = true;
                        }

                        double now = clock->time();
                        if (now - start_time >= timeout) {
                                r_err("Navigation::wait_travel: time out (%f s)", timeout);
                                _driver.moveat(0, 0);
                                success = false;
                                _stop = true;
                        }

                        recording.emplace_back(now - start_time, left, right, x, y, distance_travelled);
                        clock->sleep(0.001);
                }

                FILE* fp = fopen("/tmp/nav.csv", "w");
                if (fp) {
                        for (size_t i = 0; i < recording.size(); i++)
                                fprintf(fp, "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
                                        recording[i].time_,
                                        recording[i].left_encoder_,
                                        recording[i].right_encoder_,
                                        recording[i].position_x_,
                                        recording[i].position_y_,
                                        recording[i].distance_);
                        fclose(fp);
                }
                return success;
        }
        
        bool Navigation::do_move2(double distance, double speed)
        {
                bool success = false;
                double left, right, timestamp;
                
                if (_driver.get_encoder_values(left, right, timestamp)) {

                        // r_debug("get_encoder_values: %f %f", left, right);
                        
                        WheelOdometry odometry(_settings, left, right, timestamp);
                        
                        if (_driver.moveat(speed, speed)) {
                                
                                success = wait_travel(odometry, distance,
                                                      compute_timeout(distance, speed));
                                
                        } else {
                                r_err("Navigation::do_move2: moveat failed");
                        }
                } else {
                        r_err("Navigation::do_move2: get_encoder_values failed");
                }
                                        
                return success;
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

                        if (distance > 0.0 && speed > 0.0) {
                                // All is well, moving forward
                        } else {
                                // Moving backwards. Make sur the
                                // distance is positive and the speed
                                // negative.
                                distance = fabs(distance);
                                speed = -fabs(speed);
                        }
                        
                        if (_driver.stop()) {
                                
                                success = do_move2(distance, speed);
                                
                        } else {
                                r_err("Navigation::do_move: stop failed");
                        }
                        
                } else {
                        r_err("Navigation::do_move: invalid speed or distance: "
                              "speed=%f, distance=%f", speed, distance);
                }
                
                // A bit of a hack: in any case, make sure that the
                // rover comes to a complete standstill after 100 ms.
                _driver.stop();

                return success;
        }


        bool Navigation::moveat(double left, double right)
        {
                SynchronizedCodeBlock sync(_mutex);
                bool success = false;
                if (_status == MOVEAT_CAPABLE) 
                        success = _driver.moveat(left, right);
                else
                        r_warn("Navigation::moveat: still moving");
                return success;
        }
        
        bool Navigation::move(double distance, double speed)
        {
                bool success = false;
                {
                        SynchronizedCodeBlock sync(_mutex);
                        if (_status == MOVEAT_CAPABLE) {
                                _status = MOVING;
                        } else {
                                r_warn("Navigation::move: already moving");
                        }
                }
                        
                if (_status == MOVING)
                        success = do_move(distance, speed);
                        
                _status = MOVEAT_CAPABLE;
                        
                return success;
        }
        
        bool Navigation::stop()
        {
                // This flag should assure that we break out
                // the wait_travel loop.
                _stop = true;
                bool success = _driver.stop();
                _status = MOVEAT_CAPABLE;
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
