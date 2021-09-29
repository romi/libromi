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
#include "rover/DoubleSteering.h"

namespace romi {
        
        DoubleSteering::DoubleSteering(ISteering& first, ISteering& second)
                : first_(first),
                  second_(second)
        {
        }

        bool DoubleSteering::enable()
        {
                return first_.enable() && second_.enable();
        }
        
        bool DoubleSteering::disable()
        {
                return first_.disable() && second_.disable();
        }
        
        bool DoubleSteering::stop()
        {
                return first_.stop() && second_.stop();
        }
        
        bool DoubleSteering::drive(double speed, SteeringData steering)
        {
                return first_.drive(speed, steering)
                        && second_.drive(speed, steering);
        }
        
        bool DoubleSteering::forward(double speed)
        {
                return first_.forward(speed)
                        && second_.forward(speed);
        }
        
        bool DoubleSteering::turn(double speed, double radius)
        {
                return first_.turn(speed, radius)
                        && second_.turn(speed, radius);
        }

    bool DoubleSteering::initialise() {

        return first_.initialise()
                        && second_.initialise();;
    }
}
