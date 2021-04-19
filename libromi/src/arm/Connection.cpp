/*
  libromi

  Copyright (C) 2019 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  libromi is the base library for the Romi devices.

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
#include <r.h>
#include "Connection.h"

namespace romi::arm {

        Connection::Connection(MMAL_PORT_T *out, MMAL_PORT_T *in)
                : ptr_(nullptr)
        {
                MMAL_STATUS_T status;
                uint32_t flags = (MMAL_CONNECTION_FLAG_TUNNELLING
                                  | MMAL_CONNECTION_FLAG_ALLOCATION_ON_INPUT);
                status =  mmal_connection_create(&ptr_, out, in, flags);
                assert_status("Connection::Connection: create", status);
                status =  mmal_connection_enable(ptr_);
                assert_status("Connection::Connection: enable", status);
        }
        
        Connection::~Connection()
        {
                if (ptr_) 
                        mmal_connection_destroy(ptr_);
        }
}

