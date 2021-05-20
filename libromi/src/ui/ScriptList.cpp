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

#include <stdexcept>
#include <log.h>
#include "ui/ScriptList.h"

namespace romi {
                
        ScriptList::ScriptList(const std::string& path)
        {
                try {
                        
                        load_scripts(path);
                
                } catch (JSONError& je) {
                        r_err("ScriptList: JSON error while "
                              "loading scripts: %s", je.what());
                        throw std::runtime_error("Invalid script");
                        
                } catch (std::runtime_error& re) {
                        r_err("ScriptList: Error while converting "
                              "scripts: %s", re.what());
                        throw std::runtime_error("Invalid script");
                }
        }
                
        ScriptList::ScriptList(JsonCpp& json)
        {
                try {
                        
                        convert_scripts(json);
                
                } catch (JSONError& je) {
                        r_err("ScriptList: JSON error while "
                              "loading scripts: %s", je.what());
                        throw std::runtime_error("Invalid script");
                        
                } catch (std::runtime_error& re) {
                        r_err("ScriptList: Error while converting "
                              "scripts: %s", re.what());
                        throw std::runtime_error("Invalid script");
                }
        }
        
        void ScriptList::load_scripts(const std::string& path)
        {
                JsonCpp scripts = JsonCpp::load(path.c_str());
                convert_scripts(scripts);
        }
        
        void ScriptList::convert_scripts(JsonCpp& scripts)
        {
                for (size_t index = 0; index < scripts.length(); index++) {
                        JsonCpp script = scripts[index];
                        convert_script(script);
                }                        
        }
        
        void ScriptList::convert_script(JsonCpp& script)
        {
                const char *id = (const char *) script["id"];
                const char *title = (const char *) script["title"];
                
                push_back(Script(id, title));
                convert_script_actions(back(), script);                        
        }

        void ScriptList::convert_script_actions(Script& script,
                                                JsonCpp& json_script)
        {
                JsonCpp actions = json_script.array("actions");
                for (size_t i = 0; i < actions.length(); i++) {
                        JsonCpp action = actions[i];
                        convert_action(script, action);
                }
        }

        void ScriptList::convert_action(Script& script, JsonCpp& action)
        {
                std::string type = (const char *) action["action"];
                if (type == Action::kMoveCommand)
                        convert_move(script, action);
                else if (type == Action::kHoeCommand)
                        convert_hoe(script);
                else if (type == Action::kHomingCommand)
                        convert_homing(script);
                else if (type == Action::kStartRecordingCommand)
                        convert_start_recording(script);
                else if (type == Action::kStopRecordingCommand)
                        convert_stop_recording(script);
        }
        
        void ScriptList::convert_move(Script& script, JsonCpp& action)
        {
                double distance = (double) action["distance"];
                double speed = (double) action["speed"];
                
                assure_move_params(distance, speed);
                script.append(Action(Action::Move, distance, speed));
        }

        void ScriptList::assure_move_params(double distance, double speed)
        {
                if (fabs(distance) > 100.0) {
                        r_err("ScriptList: invalid distance for move: "
                              "%0.2f", distance);
                        throw std::runtime_error("Invalid distance");
                }
                if (fabs(speed) > 1.0) {
                        r_err("ScriptList: invalid speed for move: "
                              "%0.2f", speed);
                        throw std::runtime_error("Invalid speed");
                }
        }
        
        void ScriptList::convert_hoe(Script& script)
        {
                script.append(Action(Action::Hoe));
        }
        
        void ScriptList::convert_homing(Script& script)
        {
                script.append(Action(Action::Homing));
        }
        
        void ScriptList::convert_start_recording(Script& script)
        {
                script.append(Action(Action::StartRecording));
        }
        
        void ScriptList::convert_stop_recording(Script& script)
        {
                script.append(Action(Action::StopRecording));
        }
}
