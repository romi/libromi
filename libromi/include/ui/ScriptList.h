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
#ifndef _ROMI_SCRIPT_LIST_H
#define _ROMI_SCRIPT_LIST_H

#include <string>
#include <vector>
#include <JsonCpp.h>

namespace romi {
        
        struct Action
        {
                // FIXME: This enum is specific to the weeding
                // rover. And this is a generic header for all
                // devices.
                enum ActionType { Move, Hoe, Homing, StartRecording, StopRecording };

                static constexpr const char *kMoveCommand = "move";
                static constexpr const char *kHoeCommand = "hoe";
                static constexpr const char *kHomingCommand = "homing";
                static constexpr const char *kStartRecordingCommand = "start-recording";
                static constexpr const char *kStopRecordingCommand = "stop-recording";
                
                ActionType type;
                double params[2];

                Action(ActionType atype) : type(atype){
                        params[0] = 0.0;
                        params[1] = 0.0;
                }

                Action(ActionType atype, double param1, double param2)  : type(atype){
                        params[0] = param1;
                        params[1] = param2;
                }

        };

        struct Script
        {
                std::string id;
                std::string title;
                std::vector<Action> actions;

                Script(const char *id_, const char *title_)  : id(id_), title(title_), actions(){
                }

                void append(Action action) {
                        actions.push_back(action);
                }
        };

        class ScriptList final : public std::vector<Script>
        {
        protected:
                void load_scripts(const std::string& path);
                void convert_scripts(JsonCpp& scripts);
                void convert_script(JsonCpp& script);
                void convert_script_actions(Script& script, JsonCpp& json_script);
                void convert_action(Script& script, JsonCpp& action);
                void convert_move(Script& script, JsonCpp& action);
                void assure_move_params(double distance, double speed);
                void convert_hoe(Script& script);
                void convert_homing(Script& script);
                void convert_start_recording(Script& script);
                void convert_stop_recording(Script& script);
                
        public:
                
                explicit ScriptList(const std::string& path);
                explicit ScriptList(JsonCpp& json);
                ~ScriptList() = default; // We are final so no virtual destructor.
        };
}

#endif // _ROMI_SCRIPT_LIST_H
