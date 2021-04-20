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
#ifndef _LIBROMI_ENCODER_COMPONENT_
#define _LIBROMI_ENCODER_COMPONENT_

#include <stdint.h>
#include <MemBuffer.h>
#include "Component.h"
#include "Semaphore.h"

namespace romi::arm {
        
        class EncoderComponent : public Component {
        protected:
                MMAL_PORT_T *input_; 
                MMAL_PORT_T *output_;
                MMAL_POOL_T *pool_;
                Semaphore semaphore_;
                rpp::MemBuffer buffer_;
                bool capture_failed_;
                
        public:
                EncoderComponent();
                ~EncoderComponent() override;
                
                void prepare_capture();
                void finish_capture();
                rpp::MemBuffer& get_buffer();
                
                MMAL_PORT_T *get_input_port() {
                        return input_;
                }
                                
        protected:

                static void encoder_output_callback(MMAL_PORT_T *port,
                                                    MMAL_BUFFER_HEADER_T *buffer);
                void handle_output_callback(MMAL_BUFFER_HEADER_T *buffer);

                void assert_input_and_output();
                void set_format();
                void set_jpeg_quality();
                void disable_jpeg_restart();
                void disable_thumbnail();
                void create_header_pool();
                void disable_exif();
                void post_capture();
                void wait_capture();
                void enable_output();
                void send_buffers();
                void disable_output();
                void assert_successful_capture();
                
        private:
                EncoderComponent(EncoderComponent& other) = default;
                EncoderComponent& operator=(const EncoderComponent& other) = default;
        };
}

#endif // _LIBROMI_ENCODER_COMPONENT_
