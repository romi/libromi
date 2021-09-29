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
#ifndef __ROMI_DIFFERENTIAL_STEERING_H
#define __ROMI_DIFFERENTIAL_STEERING_H

#include "rover/ISteering.h"
#include "api/IMotorDriver.h"
#include "rover/NavigationSettings.h"

namespace romi {
        
        class DifferentialSteering : public ISteering
        {
        protected:
                IMotorDriver& motors_;
                NavigationSettings& settings_;
                double left_speed_;
                double right_speed_;

                bool moveat(double left, double right);

        public:
                DifferentialSteering(IMotorDriver& driver,
                                     NavigationSettings& settings);
                ~DifferentialSteering() override = default;
                
                bool enable() override;
                bool disable() override;
                bool stop() override;
                bool drive(double speed, SteeringData steering) override;
                bool forward(double speed) override;
                bool turn(double speed, double radius) override;
                bool initialise() override;
        };
}

#endif // __ROMI_DIFFERENTIAL_STEERING_H
