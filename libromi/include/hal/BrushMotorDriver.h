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

                void parse(JsonCpp &params) {
                        max_signal = (int) params.num("maximum_signal_amplitude");
                        use_pid = params.boolean("use_pid");
                        kp = params.get("pid").num("kp");
                        ki = params.get("pid").num("ki");
                        kd = params.get("pid").num("kd");
                        dir_left = (int) params.get("encoder_directions").num("left");
                        dir_right = (int) params.get("encoder_directions").num("right");
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
                void record_pid();
                void record_pid_main();
                void store_pid_recordings(std::vector<PidStatus>& recording);

        public:

                BrushMotorDriver(std::unique_ptr<romiserial::IRomiSerialClient>& serial,
                                 JsonCpp &config,
                                 int encoder_steps,
                                 double max_revolutions_per_sec);
                
                ~BrushMotorDriver() override {
                        recording_pid_ = false;
                }

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
        };
}

#endif // __ROMI_BRUSH_MOTORDRIVER_H
