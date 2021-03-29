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
#include <thread>
#include <r.h>
#include "fake/FakeScriptEngine.h"

namespace romi {

        FakeScriptEngine::FakeScriptEngine(ScriptList& scripts, int event)
                : _scripts(scripts),
                  _finished_event(event),
                  _send_finished_event(false) {
        }
                
        void FakeScriptEngine::_run_script(FakeScriptEngine *engine)
        {
                engine->run_script();
        }
        
        void FakeScriptEngine::run_script()
        {
                r_debug("FakeScriptEngine::run_script");
                clock_sleep(10.0);
                _send_finished_event = true;                
                r_debug("FakeScriptEngine::run_script: finished");
        }

        void FakeScriptEngine::execute_script(Rover& target, size_t id)
        {
                (void) target;
                r_debug("RoverScriptEngine::execute_script: '%d'", id);
                std::thread t(_run_script, this); 
                t.detach();
        }

        int FakeScriptEngine::get_next_event()
        {
                int event = 0;
                if (_send_finished_event) {
                        event = _finished_event;
                        _send_finished_event = false;
                }
                return event;
        }
}
