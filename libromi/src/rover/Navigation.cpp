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
#include "rover/WheelOdometry.h"
#include "rover/Navigation.h"

namespace romi {

        struct NavRecording
        {
                double time_;
                double left_encoder_;
                double right_encoder_;
                double position_x_;
                double position_y_;
                double left_speed_;
                double right_speed_;
                double distance_;

                NavRecording(double t, double left, double right,
                             double x, double y,
                             double vl, double vr,
                             double d)
                        : time_(t),
                          left_encoder_(left),
                          right_encoder_(right),
                          position_x_(x),
                          position_y_(y),
                          left_speed_(vl),
                          right_speed_(vr),
                          distance_(d) {
                };
        };

        Navigation::Navigation(IMotorDriver &driver, NavigationSettings &settings)
                : driver_(driver),
                  settings_(settings),
                  mutex_(),
                  status_(MOVEAT_CAPABLE),
                  stop_(false)
        {
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
                        try_travel(odometry, speed, distance,
                                   compute_timeout(distance, speed));
                        success = true;
                        
                } catch (const std::runtime_error& re) {
                        r_err("Navigation::travel: %s", re.what());
                }
                
                stop();
                
                return success;
        }

        double Navigation::compute_timeout(double distance, double speed)
        {
                double timeout = 0.0;
                if (speed != 0.0) {
                        double relative_speed = settings_.maximum_speed * speed;
                        double time = distance / fabs(relative_speed);
                        timeout = 1.5 * time;
                }
                return timeout;
        }

        // TODO: Spin off a seperate thread so the main event loop can continue?
        void Navigation::try_travel(WheelOdometry& pose, double speed,
                                    double distance, double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                std::vector<NavRecording> recording;
                double left_speed = speed;
                double right_speed = speed;
                
                stop_ = false;
                
                while (!stop_) {
                        
                        if (!driver_.moveat(left_speed, right_speed)) {
                                r_err("Navigation::travel: moveat failed");
                                throw std::runtime_error("Moveat failed");
                        }

                        // TODO: HANDLE USER INPUT AND HANDLE STOP
                        // REQUESTED USER-REQUESTED

                        if (!pose.update_estimation()) {
                                r_err("Navigation::travel: pose estimation failed");
                                throw std::runtime_error("Pose estimation failed");
                        }
                        
                        v3 location = pose.get_location();
                        double distance_travelled = location.norm(); // FIXME!!!
                        
                        if (distance_travelled >= distance) {
                                break;
                        }

                        double now = clock->time();
                        if (now - start_time >= timeout) {
                                r_err("Navigation::travel: time out (%f s)", timeout);
                                throw std::runtime_error("Timeout");
                        }

                        v3 encoders = pose.get_encoders();
                        recording.emplace_back(now - start_time,
                                               encoders.x(), encoders.y(),
                                               location.x(), location.y(),
                                               left_speed, right_speed,
                                               distance_travelled);
                        //clock->sleep(0.001);
                }

                FILE* fp = fopen("/tmp/nav.csv", "w");
                if (fp) {
                        for (size_t i = 0; i < recording.size(); i++)
                                fprintf(fp,
                                        "%.6f\t%.6f\t%.6f\t%.6f\t%.6f"
                                        "\t%.6f\t%.6f\t%.6f\n",
                                        recording[i].time_,
                                        recording[i].left_encoder_,
                                        recording[i].right_encoder_,
                                        recording[i].position_x_,
                                        recording[i].position_y_,
                                        recording[i].left_speed_,
                                        recording[i].right_speed_,
                                        recording[i].distance_);
                        fclose(fp);
                }
        }

        bool Navigation::moveat(double left, double right)
        {
                SynchronizedCodeBlock sync(mutex_);
                bool success = false;
                if (status_ == MOVEAT_CAPABLE) 
                        success = driver_.moveat(left, right);
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
