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
#include "PiCamera.h"

namespace romi::arm {
        
        Component::Component(const char *name)
                : MMAL(), ptr_(nullptr), control_port_(nullptr)
        {
                if (mmal_component_create(name, &ptr_) != MMAL_SUCCESS) {
                        r_err("mmal_component_create failed for '%s'", name);
                        throw std::runtime_error("mmal_component_create failed");
                }
                control_port_ = ptr_->control;
        }

        Component::~Component()
        {
                if (ptr_) {
                        mmal_component_disable(ptr_);
                        mmal_component_destroy(ptr_);
                }
        }

        void Component::enable()
        {
                MMAL_STATUS_T status = mmal_component_enable(ptr_);
                assert_status("enable", status);
        }
        
        void Component::control_callback(MMAL_PORT_T *port,
                                         MMAL_BUFFER_HEADER_T *buffer)
        {
                Component *component = (Component *) port->userdata;
                component->handle_control_callback(buffer);
        }
        
        void Component::handle_control_callback(MMAL_BUFFER_HEADER_T *buffer)
        {
                mmal_buffer_header_release(buffer);
        }
        
        void Component::enable_control_port()
        {
                MMAL_STATUS_T status;
                control_port_->userdata = (struct MMAL_PORT_USERDATA_T *) this;
                status = mmal_port_enable(control_port_,
                                          Component::control_callback);
                assert_status("enable_control_port", status);
        }
        
        void Component::set_control_parameter(const char *caller_name,
                                              const MMAL_PARAMETER_HEADER_T *param)
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set(control_port_, param);
                assert_status(caller_name, status);
        }

        void Component::set_control_parameter(const char *caller_name,
                                              uint32_t id,
                                              MMAL_RATIONAL_T value)
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set_rational(control_port_, id, value);
                assert_status(caller_name, status);
        }

        void Component::set_control_parameter(const char *caller_name,
                                              uint32_t id,
                                              uint32_t value)
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set_uint32(control_port_, id, value);
                assert_status(caller_name, status);
        }

        void Component::set_control_parameter(const char *caller_name,
                                              uint32_t id,
                                              int32_t value)
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set_int32(control_port_, id, value);
                assert_status(caller_name, status);
        }

        void Component::set_control_parameter(const char *caller_name,
                                              uint32_t id,
                                              bool value)
        {
                MMAL_STATUS_T status;
                status = mmal_port_parameter_set_boolean(control_port_, id,
                                                         (MMAL_BOOL_T) value);
                assert_status(caller_name, status);
        }
        
        void Component::check_disable_port(MMAL_PORT_T *port)
        {
                if (port && port->is_enabled)
                        mmal_port_disable(port);
        }
}

