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
#include "CNCProxy.h"

namespace romi {
        
        void CNCProxy::get_range(CNCRange &range)
        {
                r_debug("CNCProxy::get_range");
                JSON command = JSON::parse("{\"command\": \"get-range\"}");
                JSON reply = _rpc_handler->execute(command);
                range.init(reply.get("range"));
        }
        
        void CNCProxy::moveto(double x, double y, double z, double v)
        {
                r_debug("CNCProxy::moveto");
                membuf_t *buf = new_membuf();
                
                membuf_printf(buf, "{\"command\": \"moveto\"");
                if (x != UNCHANGED)
                        membuf_printf(buf, ", \"x\": %.5f", x);
                if (y != UNCHANGED)
                        membuf_printf(buf, ", \"y\": %.5f", y);
                if (z != UNCHANGED)
                        membuf_printf(buf, ", \"z\": %.5f", z);
                membuf_printf(buf, ", \"speed\": %.2f }", v);
                membuf_append_zero(buf);
                
                JSON command = JSON::parse(membuf_data(buf));

                delete_membuf(buf);
                
                _rpc_handler->execute(command);
        }

        void CNCProxy::spindle(double speed)
        {
                r_debug("CNCProxy::spindle");
                JSON command = JSON::construct("{\"command\": \"spindle\", "
                                               "\"speed\": %.2f}", speed);
                _rpc_handler->execute(command);
        }

        void CNCProxy::travel(Path &path, double relative_speed)
        {
                r_debug("CNCProxy::travel");
                
                r_debug("CNCProxy::travel @1");
                
                // convert path to json
                membuf_t *buf = new_membuf();

                r_debug("CNCProxy::travel @2");
                
                membuf_printf(buf, "{\"command\": \"travel\", \"path\": [");

                r_debug("CNCProxy::travel @3");
                
                for (size_t i = 0; i < path.size(); i++) {

                        r_debug("CNCProxy::travel @4");
                
                        if (i > 0)
                                membuf_printf(buf, ",");

                        membuf_printf(buf, "[%.4f,%.4f,%.4f]",
                                      path[i].x, path[i].y, path[i].z);
                }

                r_debug("CNCProxy::travel @5");
                
                membuf_printf(buf, "]");
                membuf_printf(buf, ", \"speed\": %f", relative_speed);
                membuf_printf(buf, "}");

                r_debug("CNCProxy::travel @6");
                
                membuf_append_zero(buf);

                printf("CNCProxy::travel: membuf length=%d\n", membuf_len(buf));
                printf("CNCProxy::travel: membuf data=%s\n", membuf_data(buf));

                r_debug("CNCProxy::travel @7");
                
                //r_debug("CNCProxy::travel: command=%s", membuf_data(buf));
                
                JSON command = JSON::parse(membuf_data(buf));

                r_debug("CNCProxy::travel @8");

                delete_membuf(buf);
                
                _rpc_handler->execute(command);                

                r_debug("CNCProxy::travel @9");
                
        }

        void CNCProxy::homing()
        {
                r_debug("CNCProxy::homing");
                JSON command = JSON::parse("{\"command\": \"homing\"}");
                _rpc_handler->execute(command);
        }

        void CNCProxy::stop_execution()
        {
                r_debug("CNCProxy::stop_execution");
                JSON command = JSON::parse("{\"command\": \"stop\"}");
                _rpc_handler->execute(command);
        }

        void CNCProxy::continue_execution()
        {
                r_debug("CNCProxy::continue_execution");
                JSON command = JSON::parse("{\"command\": \"continue\"}");
                _rpc_handler->execute(command);
        }

        void CNCProxy::reset()
        {
                r_debug("CNCProxy::reset");
                JSON command = JSON::parse("{\"command\": \"reset\"}");
                _rpc_handler->execute(command);
        }

        
}