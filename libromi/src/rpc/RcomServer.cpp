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
#include <string.h>
#include <MessageHub.h>
#include "rpc/RcomServer.h"

namespace romi {

        std::unique_ptr<IRPCServer> RcomServer::create(const std::string& topic,
                                                       IRPCHandler &handler)
        {
                auto listener = std::make_unique<RcomMessageHandler>(handler);
                std::unique_ptr<rcom::IMessageHub> hub
                        = std::make_unique<rcom::MessageHub>(topic, *listener);
                return std::make_unique<RcomServer>(hub, listener);
        }
        
        RcomServer::RcomServer(std::unique_ptr<rcom::IMessageHub>& hub,
                               std::unique_ptr<RcomMessageHandler>& listener)
                : hub_(), listener_()
        {
                listener_ = std::move(listener);
                hub_ = std::move(hub);
        }

        void RcomServer::handle_events()
        {
                hub_->handle_events();
        }
}
