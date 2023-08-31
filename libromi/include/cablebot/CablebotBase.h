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
#ifndef __ROMI_CABLEBOTBASE_H
#define __ROMI_CABLEBOTBASE_H

#include <stdexcept>
#include <memory>
#include <RomiSerialClient.h>
#include "hal/ICameraMount.h"
#include "api/IGimbal.h"


namespace romi {
        
        class CablebotBase : public ICameraMount
        {
        protected:

                static const constexpr double kPrecision = 36.0;
                static const constexpr double kDiameter = 0.0505;
                
                std::unique_ptr<romiserial::IRomiSerialClient> base_serial_;
                std::unique_ptr<IGimbal> gimbal_;
                CNCRange range_xyz_;
                Range range_angles_;
                double diameter_;
                double circumference_;

                void validate_xyz_coordinates(double x, double y, double z);
                void validate_angles(double ax, double ay, double az);
                void validate_speed(double v);
                bool send_base_command(const char *command);
                int16_t position_to_steps(double x);
                double steps_to_position(double steps);
                bool enable_driver();
                bool disable_driver();
                void try_moveto(double x, double ax, double relative_speed);
                void base_moveto(double x, double relative_speed);
                void gimbal_moveto(double ax, double relative_speed);
                void synchronize_with_base(double timeout);
                bool is_base_on_target();
                bool get_base_position(v3& xyz); 
                bool synchronize_base(double timeout_seconds);
                int on_position();

        public:
                CablebotBase(std::unique_ptr<romiserial::IRomiSerialClient>& base_serial,
                             std::unique_ptr<IGimbal>& gimbal);
                virtual ~CablebotBase() = default;


                bool homing() override;
                uint8_t count_relays() override;
                bool set_relay(uint8_t index, bool value) override;
                bool get_position(v3& xyz, v3& angles) override; 
                bool get_range(CNCRange &xyz, IRange &angles) override;
                bool moveto(double x, double y, double z,
                            double ax, double ay, double az,
                            double relative_speed) override;
                bool synchronize(double timeout_seconds) override; 
                
                // IActivity interface
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // Power device interface
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_CABLEBOTBASE_H
