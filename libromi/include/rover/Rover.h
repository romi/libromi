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

#ifndef __ROMI_ROVER_H
#define __ROMI_ROVER_H

#include "api/IInputDevice.h"
#include "api/IDisplay.h"
#include "api/INotifications.h"
#include "api/IWeeder.h"
#include "api/INavigation.h"
#include "api/StateMachine.h"
#include "api/IEventTimer.h"
#include "api/IImager.h"
#include "ui/Menu.h"
#include "ui/IScriptEngine.h"
#include "ISpeedController.h"

namespace romi {
        
        class Rover
        {
        public:
                IInputDevice& input_device;
                IDisplay& display;
                ISpeedController& speed_controller;
                INavigation& navigation;
                IEventTimer& event_timer;
                Menu& menu;
                IScriptEngine<Rover>& script_engine;
                INotifications& notifications;
                IWeeder& weeder;
                IImager& imager;
                
                Rover(IInputDevice& input_device_,
                      IDisplay& display_,
                      ISpeedController& speed_controller_,
                      INavigation& navigation_,
                      IEventTimer& event_timer,
                      Menu& menu,
                      IScriptEngine<Rover>& script_engine,
                      INotifications& notifications,
                      IWeeder& weeder,
                      IImager& imager);
                
                virtual ~Rover() = default;

                bool reset();
        };
}

#endif // __ROMI_ROVER_H
