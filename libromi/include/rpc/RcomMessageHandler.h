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

#ifndef __ROMI_RCOM_MESSAGE_HANDLER_H
#define __ROMI_RCOM_MESSAGE_HANDLER_H

#include <memory>
#include <IMessageListener.h>
#include "rpc/IRPCHandler.h"

namespace romi {
        
        class RcomMessageHandler : public rcom::IMessageListener
        {
        protected:
                IRPCHandler& handler_;

                JsonCpp parse_request(rpp::MemBuffer& message, RPCError& error);
                const char *get_method(JsonCpp& request, RPCError& error);

                json_object_t construct_response(RPCError& error, JsonCpp& result);
                json_object_t construct_response(RPCError& error);
                json_object_t construct_response(int code, const char *message);
                
        public:
                RcomMessageHandler(IRPCHandler& handler);
                virtual ~RcomMessageHandler() = default;

                void onmessage(rcom::IWebSocket& websocket,
                               rpp::MemBuffer& message) override;
        };
}

#endif // __ROMI_RCOM_MESSAGE_HANDLER_H
