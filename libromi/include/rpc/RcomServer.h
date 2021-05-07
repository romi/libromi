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

#ifndef __ROMI_RCOM_SERVER_H
#define __ROMI_RCOM_SERVER_H

#include <memory>
#include <IMessageHub.h>
#include "rpc/IRPCServer.h"
#include "rpc/IRPCHandler.h"
#include "rpc/RcomMessageHandler.h"

namespace romi {
        
        class RcomServer : public IRPCServer
        {
        protected:
                std::unique_ptr<rcom::IMessageHub> hub_;

        public:
                
                static std::unique_ptr<IRPCServer> create(const std::string& topic,
                                                          IRPCHandler &handler);
                
                RcomServer(std::unique_ptr<rcom::IMessageHub>& hub);
                virtual ~RcomServer() = default;

                void handle_events();
        };
}

#endif // __ROMI_RCOM_SERVER_H
