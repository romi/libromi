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
#ifndef __ROMI_CNCANDGIMBAL_H
#define __ROMI_CNCANDGIMBAL_H

#include <memory>
#include "hal/ICameraMount.h"
#include "api/ICNC.h"
#include "api/IGimbal.h"

namespace romi {
        
        class CNCAndGimbal : public ICameraMount
        {
        protected:
                std::unique_ptr<ICNC> cnc_;
                std::unique_ptr<IGimbal> gimbal_;
                
        public:
                CNCAndGimbal(std::unique_ptr<ICNC>& cnc,
                             std::unique_ptr<IGimbal>& gimbal);
                ~CNCAndGimbal() override = default;

                bool homing() override; 
                uint8_t count_relays() override;
                bool set_relay(uint8_t index, bool value) override;
                bool get_position(v3& xyz, v3& angles) override;
                bool get_range(CNCRange &xyz, IRange &angles) override;
                bool moveto(double x, double y, double z,
                            double phi_x, double phi_y, double phi_z,
                            double relative_speed) override;
                bool synchronize(double timeout_seconds) override;
                
                // IPowerDevice
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
                
                // IActivity
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;
        };
}

#endif // __ROMCNCANDGIMBAL_H
