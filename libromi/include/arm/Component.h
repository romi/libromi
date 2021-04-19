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
#ifndef _LIBROMI_COMPONENT_H_
#define _LIBROMI_COMPONENT_H_

#include <stdint.h>
#include "MMAL.h"

namespace romi::arm {
        
        class Component : public MMAL {
        public:
                MMAL_COMPONENT_T *ptr_;
                MMAL_PORT_T *control_port_;
                
                Component(const char* name);
                virtual ~Component();

                void enable();
                
        protected:
                
                static void control_callback(MMAL_PORT_T *port,
                                             MMAL_BUFFER_HEADER_T *buffer);
                virtual void handle_control_callback(MMAL_BUFFER_HEADER_T *buffer);

                void enable_control_port();
                void check_disable_port(MMAL_PORT_T *port);
                
                void set_control_parameter(const char *caller_name,
                                           const MMAL_PARAMETER_HEADER_T *param);
                void set_control_parameter(const char *caller_name,
                                           uint32_t id,
                                           MMAL_RATIONAL_T value);
                void set_control_parameter(const char *caller_name,
                                           uint32_t id,
                                           uint32_t value);
                void set_control_parameter(const char *caller_name,
                                           uint32_t id,
                                           int32_t value);
                void set_control_parameter(const char *caller_name,
                                           uint32_t id,
                                           bool value);
        private:
                Component(Component& other) = default;
                Component& operator=(const Component& other) = default;
        };
}

#endif // _LIBROMI_COMPONENT_H_
