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

#include "api/EventTimer.h"

namespace romi {

        void EventTimer::reset()
        {
                _event_time = 0.0;
        }

        bool EventTimer::has_timed_out()
        {
                return (_event_time != 0.0
                        && _clock->time() >= _event_time);
        }

        int EventTimer::get_next_event()
        {
                int event = 0;
                if (has_timed_out()) {
                        event = _event;
                        reset();
                }
                return event;
        }
        
        void EventTimer::set_timeout(double timeout)
        {
                _event_time = _clock->time() + timeout;
        }
}

