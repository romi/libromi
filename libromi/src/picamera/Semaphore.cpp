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
#include "picamera/Semaphore.h"

namespace romi::arm {

        Semaphore::Semaphore() : s_()
        {
                VCOS_STATUS_T status;
                status = vcos_semaphore_create(&s_, "PiCamera", 0);
                if (status != VCOS_SUCCESS) {
                        r_err("vcos_semaphore_create failed");
                        throw std::runtime_error("vcos_semaphore_create failed");
                }
        }

        Semaphore::~Semaphore()
        {
                vcos_semaphore_delete(&s_);
        }

        void Semaphore::post()
        {
                vcos_semaphore_post(&s_);
        }
        
        void Semaphore::wait()
        {
                vcos_semaphore_wait(&s_);
        }
}

