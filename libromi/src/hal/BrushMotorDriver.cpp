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
#include <functional>
#include <log.h>
#include <util.h>
#include <ClockAccessor.h>
#include "hal/BrushMotorDriver.h"

namespace romi {
        
        BrushMotorDriver::BrushMotorDriver(std::unique_ptr<romiserial::IRomiSerialClient>& serial,
                                           JsonCpp &config,
                                           int encoder_steps,
                                           double max_revolutions_per_sec)
                : serial_(),
                  settings_(),
                  recording_pid_(false),
                  pid_thread_()
        {
                //_serial.set_debug(true);
                serial_ = std::move(serial);
                if (!disable_controller()
                    || !configure_controller(config, encoder_steps, max_revolutions_per_sec)
                    || !enable_controller()) {
                        throw std::runtime_error("BrushMotorDriver: "
                                                 "Initialization failed");
                }
        }
        
        BrushMotorDriver::~BrushMotorDriver()
        {
                stop_recording_pid();
        }
                
        bool BrushMotorDriver::configure_controller(JsonCpp &config, int steps,
                                                    double max_revolutions_per_sec)
        {
                settings_.parse(config);

                char command[100];
                rprintf(command, 100, "C[%d,%d,%d,%d,%d,%d,%d,%d,%d]",
                        steps,
                        (int) (100.0 * max_revolutions_per_sec),
                        settings_.max_signal,
                        settings_.use_pid? 1 : 0,
                        (int) (1000.0 * settings_.kp),
                        (int) (1000.0 * settings_.ki),
                        (int) (1000.0 * settings_.kd),
                        settings_.dir_left,
                        settings_.dir_right);
                        
                JsonCpp response;
                serial_->send(command, response);
                return check_response(command, response);
        }
        
        bool BrushMotorDriver::check_response(const char *command,
                                              JsonCpp& response)
        {
                bool success = (response.num(romiserial::kStatusCode) == 0);
                if (!success) {
                        const char *message = "No message";
                        if (response.length() > 1)
                                message = response.str(romiserial::kErrorMessage);
                        r_warn("BrushMotorDriver: command %s returned error: %s",
                               command, message);
                }
                return success;
        }

        bool BrushMotorDriver::enable_controller()
        {
                JsonCpp response;
                const char *command = "E[1]";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::disable_controller()
        {
                JsonCpp response;
                const char *command = "E[0]";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::stop()
        {
                JsonCpp response;
                const char *command = "X";
                serial_->send(command, response);
                return check_response(command, response);
        }

        bool BrushMotorDriver::moveat(double left, double right)
        {
                bool success = false;

                r_debug("BrushMotorDriver::moveat: %f, %f", left, right);

                if (left >= -1.0 && left <= 1.0
                    && right >= -1.0 && right <= 1.0) {
                                
                        int32_t ileft = (int32_t) (1000.0 * left);
                        int32_t iright = (int32_t) (1000.0 * right);
                                
                        char command[64];
                        rprintf(command, 64, "V[%d,%d]", ileft, iright);
                                
                        JsonCpp response;
                        serial_->send(command, response);
                        success = check_response(command, response);
                        
                } else {
                        r_warn("BrushMotorDriver::moveat: invalid speeds: %f, %f",
                               left, right);
                }

                return success;
        }

        bool BrushMotorDriver::get_encoder_values(double& left,
                                                  double& right,
                                                  double& timestamp)
        {
                JsonCpp response;
                const char *command = "e";
                serial_->send(command, response);
                bool success = check_response(command, response);
                if (success) {
                        left = response.num(1);
                        right = response.num(2);
                        timestamp = response.num(3) / 1000.0;
                }
                return success;
        }

        bool BrushMotorDriver::get_pid_values(Axis axis,
                                              double& target_speed,
                                              double& measured_speed,
                                              double& pid_output,
                                              double& pid_error_p,
                                              double& pid_error_i,
                                              double& pid_error_d,
                                              double& controller_input)
        {
                JsonCpp response;
                char command[16];
                snprintf(command, sizeof(command), "p[%d]", axis);

                serial_->send(command, response);
                bool success = check_response(command, response);
                if (success) {
                        target_speed = response.num(1);
                        measured_speed = response.num(2);
                        pid_output = response.num(3);
                        pid_error_p = response.num(4);
                        pid_error_i = response.num(5);
                        pid_error_d = response.num(6);
                        controller_input = response.num(7);
                }
                return success;
        }

        void BrushMotorDriver::start_recording_pid()
        {
                recording_pid_ = true;
                pid_thread_ = std::make_unique<std::thread>(
                        [this]() {
                                record_pid_main();
                        });
        }

        void BrushMotorDriver::stop_recording_pid()
        {
                recording_pid_ = false;
                if (pid_thread_) {
                        pid_thread_->join();
                        pid_thread_ = nullptr;
                }
        }

        void BrushMotorDriver::record_pid_main()
        {
                auto clock = rpp::ClockAccessor::GetInstance();
                double start_time = clock->time();
                std::vector<PidStatus> recording;
                
                while (recording_pid_) {
                        double now;
                        double target;
                        double measured_speed;
                        double output;
                        double error_p;
                        double error_i;
                        double error_d;
                        double controller_input;

                        now = clock->time();
                        bool success = get_pid_values(kLeftWheel, target,
                                                      measured_speed, output, error_p,
                                                      error_i, error_d, controller_input);

                        if (success) {
                                recording.emplace_back(now - start_time, target,
                                                       measured_speed, output, error_p,
                                                       error_i, error_d, controller_input);
                                if (recording.size() >= 100) {
                                        store_pid_recordings(recording);
                                }
                        }
                        
                        clock->sleep(0.050);
                }
        }

        void BrushMotorDriver::store_pid_recordings(std::vector<PidStatus>& recording)
        {
                FILE* fp = fopen("/tmp/pid.csv", "w");
                if (fp) {
                        for (size_t i = 0; i < recording.size(); i++)
                                fprintf(fp,
                                        "%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\t%.6f\n",
                                        recording[i].time_,
                                        recording[i].target_,
                                        recording[i].measured_speed_,
                                        recording[i].output_,
                                        recording[i].error_p_,
                                        recording[i].error_i_,
                                        recording[i].error_d_,
                                        recording[i].controller_input_);
                        fclose(fp);
                }
                
        }
}
