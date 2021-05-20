/*
  romi-rover

  Copyright (C) 2019-2020 Sony Computer Science Laboratories
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
#include "rover/Rover.h"

namespace romi {
        
        Rover::Rover(IInputDevice &input_device_,
                     IDisplay &display_,
                     ISpeedController &speed_controller_,
                     INavigation& navigation_,
                     IEventTimer &event_timer_,
                     Menu &menu_,
                     IScriptEngine<Rover>& script_engine_,
                     INotifications& notifications_,
                     IWeeder& weeder_,
                     IImager& imager_)
                : input_device(input_device_),
                  display(display_),
                  speed_controller(speed_controller_),
                  navigation(navigation_),
                  event_timer(event_timer_),
                  menu(menu_),
                  script_engine(script_engine_),
                  notifications(notifications_),
                  weeder(weeder_),
                  imager(imager_)
        {
        }

        bool Rover::reset()
        {
                bool success = (navigation.stop()
                                && weeder.stop());
                event_timer.reset();
                notifications.reset();
                return success;
        }
}
