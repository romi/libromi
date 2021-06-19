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
#ifndef __ROMI_BRUSH_MOTORDRIVER_H
#define __ROMI_BRUSH_MOTORDRIVER_H

#include <stdexcept>
#include <memory>
#include <atomic>
#include <thread>
#include "api/IMotorDriver.h"
#include "RomiSerialClient.h"

namespace romi {

        struct BrushMotorDriverSettings
        {
                int max_signal;
                bool use_pid;
                double kp, ki, kd;
                int dir_left;
                int dir_right;

                static constexpr const char *kMaxSignalAmpKey = "maximum-signal-amplitude"; 
                static constexpr const char *kUsePidKey = "use-pid"; 
                static constexpr const char *kPidKey = "pid"; 
                static constexpr const char *kKpKey = "kp"; 
                static constexpr const char *kKiKey = "ki"; 
                static constexpr const char *kKdKey = "kd"; 
                static constexpr const char *kEncoderDirectionsKey = "encoder-directions"; 
                static constexpr const char *kLeftKey = "left"; 
                static constexpr const char *kRightKey = "right"; 

                void parse(JsonCpp &params) {
                        max_signal = (int) params.num(kMaxSignalAmpKey);
                        use_pid = params.boolean(kUsePidKey);
                        kp = params.get(kPidKey).num(kKpKey);
                        ki = params.get(kPidKey).num(kKiKey);
                        kd = params.get(kPidKey).num(kKdKey);
                        dir_left = (int) params.get(kEncoderDirectionsKey).num(kLeftKey);
                        dir_right = (int) params.get(kEncoderDirectionsKey).num(kRightKey);
                }
        };

        struct PidStatus
        {
                double time_;
                double target_;
                double measured_speed_;
                double output_;
                double error_p_;
                double error_i_;
                double error_d_;
                double controller_input_;
                
                PidStatus(double t, double target, double measured_speed,
                          double output, double error_p, double error_i,
                          double error_d, double controller_input)
                        : time_(t),
                          target_(target),
                          measured_speed_(measured_speed),
                          output_(output),
                          error_p_(error_p),
                          error_i_(error_i),
                          error_d_(error_d),
                          controller_input_(controller_input) {
                };
        };

        struct Speeds
        {
                double time_;
                double left_absolute_;
                double right_absolute_;
                double left_normalized_;
                double right_normalized_;
                
                Speeds(double t, double left_absolute, double right_absolute,
                       double left_normalized, double right_normalized)
                        : time_(t),
                          left_absolute_(left_absolute),
                          right_absolute_(right_absolute),
                          left_normalized_(left_normalized),
                          right_normalized_(right_normalized) {
                };
        };
        
        class BrushMotorDriver : public IMotorDriver
        {
        protected:
                std::unique_ptr<romiserial::IRomiSerialClient> serial_;
                BrushMotorDriverSettings settings_;
                
                bool configure_controller(JsonCpp &config, int steps,
                                          double max_revolutions_per_sec);
                        
                bool disable_controller();
                bool enable_controller();
                bool check_response(const char *command,
                                    JsonCpp& response);

                // Debugging
                std::atomic<bool> recording_pid_;
                std::unique_ptr<std::thread> pid_thread_;
                void record_pid();
                void record_pid_main();
                void store_pid_recordings(std::vector<PidStatus>& recording);

                std::atomic<bool> recording_speeds_;
                std::unique_ptr<std::thread> speeds_thread_;
                void record_speeds();
                void record_speeds_main();
                void store_speed_recordings(std::vector<Speeds>& recording);
                bool get_speeds_values(double& left_absolute,
                                       double& right_absolute,
                                       double& left_normalized,
                                       double& right_normalized);
                
        public:

                BrushMotorDriver(std::unique_ptr<romiserial::IRomiSerialClient>& serial,
                                 JsonCpp &config,
                                 int encoder_steps,
                                 double max_revolutions_per_sec);
                
                ~BrushMotorDriver() override;

                bool stop() override;
                bool moveat(double left, double right) override;
                bool get_encoder_values(double& left, double& right,
                                        double& timestamp) override;
                
                bool get_pid_values(Axis axis,
                                    double& target_speed,
                                    double& measured_speed,
                                    double& pid_output,
                                    double& pid_error_p,
                                    double& pid_error_i,
                                    double& pid_error_d,
                                    double& controller_input) override;
                void start_recording_pid();
                void stop_recording_pid();
                void start_recording_speeds();
                void stop_recording_speeds();
        };
}

#endif // __ROMI_BRUSH_MOTORDRIVER_H
