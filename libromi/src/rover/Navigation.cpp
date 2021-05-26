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
                double left_target_speed_;
                double right_target_speed_;
                double pid_target_;
                double measured_speed_;
                double pid_output_;
                double pid_error_p_;
                double pid_error_i_;
                double pid_error_d_;
                double controller_input_;
                double distance_;

                NavRecording(double t, double left, double right,
                             double x, double y,
                             double v_target_left, double v_target_right,
                             double pid_target, double measured_speed, double pid_output,
                             double pid_error_p, double pid_error_i, double pid_error_d,
                             double controller_input, double d)
                        : time_(t),
                          left_encoder_(left),
                          right_encoder_(right),
                          position_x_(x),
                          position_y_(y),
                          left_target_speed_(v_target_left),
                          right_target_speed_(v_target_right),
                          pid_target_(pid_target),
                          measured_speed_(measured_speed),
                          pid_output_(pid_output),
                          pid_error_p_(pid_error_p),
                          pid_error_i_(pid_error_i),
                          pid_error_d_(pid_error_d),
                          controller_input_(controller_input),
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
                        success = try_travel(odometry, speed, distance,
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
                        timeout = 5.0 + 1.5 * time;

                        r_debug("Navigation::compute_timeout: v_rel=%f, v_max=%f, "
                                "v=%f, dist=%f, dt=%f, timeout=%f", relative_speed,
                                settings_.maximum_speed, speed, distance, time, timeout);
                }
                return timeout;
        }

        // TODO: Spin off a seperate thread so the main event loop can continue?
        bool Navigation::try_travel(WheelOdometry& pose, double speed,
                                    double distance, double timeout)
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                std::vector<NavRecording> recording;
                double left_speed = speed;
                double right_speed = speed;
                double distance_travelled;
                v3 location;
                bool success = false;
                
                stop_ = false;
                
                while (!stop_) {
                        
                        if (!driver_.moveat(left_speed, right_speed)) {
                                r_err("Navigation::travel: moveat failed");
                                break;
                        }

                        // TODO: HANDLE USER INPUT AND HANDLE STOP
                        // REQUESTED USER-REQUESTED

                        if (!pose.update_estimation()) {
                                r_err("Navigation::travel: pose estimation failed");
                                break;
                        }
                        
                        location = pose.get_location();
                        distance_travelled = location.norm(); // FIXME!!!
                        
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

                        double pid_target;
                        double measured_speed;
                        double pid_output;
                        double pid_error_p;
                        double pid_error_i;
                        double pid_error_d;
                        double controller_input;
                        v3 encoders = pose.get_encoders();
                        driver_.get_pid_values(IMotorDriver::kLeftWheel,
                                               pid_target, measured_speed, pid_output,
                                               pid_error_p, pid_error_i, pid_error_d, controller_input);

                        recording.emplace_back(now - start_time,
                                               encoders.x(), encoders.y(),
                                               location.x(), location.y(),
                                               left_speed, right_speed,
                                               pid_target, measured_speed, pid_output,
                                               pid_error_p, pid_error_i, pid_error_d,
                                               controller_input, distance_travelled);
                        
                        //clock->sleep(0.100);
                }

                driver_.moveat(0.0, 0.0);
                left_speed = 0.0;
                right_speed = 0.0;
                
                if (true) {
                        double end_time = clock->time() + 5.0;
                        while (true) {

                                double pid_target;
                                double measured_speed;
                                double pid_output;
                                double pid_error_p;
                                double pid_error_i;
                                double pid_error_d;
                                double controller_input;
                                v3 encoders = pose.get_encoders();
                                driver_.get_pid_values(IMotorDriver::kLeftWheel,
                                                       pid_target, measured_speed, pid_output,
                                                       pid_error_p, pid_error_i, pid_error_d, controller_input);
                                
                                double now = clock->time();

                                recording.emplace_back(now - start_time,
                                                       encoders.x(), encoders.y(),
                                                       location.x(), location.y(),
                                                       left_speed, right_speed,
                                                       pid_target, measured_speed, pid_output,
                                                       pid_error_p, pid_error_i, pid_error_d,
                                                       controller_input, distance_travelled);
                                if (now >= end_time) {
                                        break;
                                }
                                
                                //clock->sleep(0.100);
                        }
                }
                
                FILE* fp = fopen("/tmp/nav.csv", "w");
                if (fp) {
                        fprintf(fp,
                                "# time\tleft-enc\tright-enc\tpos-x\tpos-y\ttarget-l\ttarget-r\t"
                                "pid-target\tmeasured-speed\tpid-out\tpid-ep\tpid-ei\tpid-ed\tcontrol-in\tdist\n");
                        for (size_t i = 0; i < recording.size(); i++)
                                fprintf(fp,
                                        "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t"
                                        "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
                                        recording[i].time_,
                                        recording[i].left_encoder_,
                                        recording[i].right_encoder_,
                                        recording[i].position_x_,
                                        recording[i].position_y_,
                                        recording[i].left_target_speed_,
                                        recording[i].right_target_speed_,
                                        recording[i].pid_target_,
                                        recording[i].measured_speed_,
                                        recording[i].pid_output_,
                                        recording[i].pid_error_p_,
                                        recording[i].pid_error_i_,
                                        recording[i].pid_error_d_,
                                        recording[i].controller_input_,
                                        recording[i].distance_);
                        fclose(fp);
                }

                return success;
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
