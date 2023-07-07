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
#ifndef __ROMI_FAKEGIMBAL_H
#define __ROMI_FAKEGIMBAL_H

#include "api/IGimbal.h"

namespace romi {
        
        class FakeGimbal : public IGimbal
        {
        protected:
                double x_;
                double y_;
                double z_;
                
        public:
                FakeGimbal();
                virtual ~FakeGimbal() = default;
                                
                // IGimbal
                bool moveto(double x, double y, double z, double relative_speed) override;
                bool get_position(v3& position) override; 
                bool get_range(IRange& range) override; 
                bool homing() override;
                bool synchronize(double timeout_seconds);

                // IActivity
                bool pause_activity() override;
                bool continue_activity() override;
                bool reset_activity() override;

                // IPowerDevice
                bool power_up() override;
                bool power_down() override;
                bool stand_by() override;
                bool wake_up() override;
        };
}

#endif // __ROMI_FAKEGIMBAL_H
