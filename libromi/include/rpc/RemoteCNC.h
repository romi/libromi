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
#ifndef __ROMI_REMOTECNC_H
#define __ROMI_REMOTECNC_H

#include <memory>
#include <rcom/RemoteStub.h>
#include "api/ICNC.h"

namespace romi {

        class RemoteCNC : public ICNC, public rcom::RemoteStub
        {
        public:
                static constexpr const char *ClassName = "remote-cnc";
                
        public:
                explicit RemoteCNC(std::unique_ptr<rcom::IRPCClient>& rpc_client);
                ~RemoteCNC() override = default;

                bool get_range(CNCRange &range) override;
                bool get_position(v3& position) override;
                bool moveto(double x, double y, double z,
                            double relative_speed) override;
                // bool moveat(int16_t speed_x, int16_t speed_y, int16_t speed_z) override;
                bool spindle(double speed) override;
                bool travel(Path &path, double relative_speed) override;
                bool homing() override;
                uint8_t count_relays() override;
                bool set_relay(uint8_t index, bool value) override;
                bool helix(double xc, double yc, double alpha, double z,
                           double relative_speed) override;
                bool synchronize(double timeout_seconds) override; 

                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;

        };
}

#endif // __ROMI_REMOTECNC_H
