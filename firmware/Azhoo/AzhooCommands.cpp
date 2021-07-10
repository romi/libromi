/*
  Romi motor controller for brushed motors

  Copyright (C) 2021 Sony Computer Science Laboratories
  Author(s) Peter Hanappe

  Azhoo is free software: you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see
  <http://www.gnu.org/licenses/>.

 */

#include <stdio.h>
#include "AzhooCommands.h"
#include "AzhooVersion.h"

using namespace romiserial;

const static MessageHandler handlers[] = {
        { '?', 0, false, send_info },
        { 'e', 0, false, send_encoders },
        { 'C', 9, false, handle_configure },
        { 'E', 1, false, handle_enable },
        { 'V', 2, false, handle_moveat },
        { 'X', 0, false, handle_stop },
        // { 'p', 1, false, send_pid },
        // { 'v', 0, false, send_speeds },
        // { 'S', 0, false, send_status },
        // { 'c', 0, false, send_configuration },
};

static IAzhoo *azhoo_ = nullptr;
static IRomiSerial *romi_serial_ = nullptr;
static char reply_buffer[100];

const char *kErrBadState = "Bad state";

void setup_commands(IAzhoo *azhoo, IRomiSerial *romiSerial)
{
        azhoo_ = azhoo;
        romi_serial_ = romiSerial;
        if (romi_serial_)
                romi_serial_->set_handlers(handlers, sizeof(handlers) / sizeof(MessageHandler));
}

void handle_commands()
{
        if (azhoo_ != nullptr
            && romi_serial_ != nullptr)
                romi_serial_->handle_input();
}

void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) args;
        (void) string_arg;
        
        romiSerial->send("[0,\"Azhoo\",\"" kAzhooVersion "\","
                         "\"" __DATE__ " " __TIME__ "\"]"); 
}

void send_encoders(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) args;
        (void) string_arg;
        int32_t left;
        int32_t right;
        uint32_t time;
        azhoo_->get_encoders(left, right, time);

#if ARDUINO
        snprintf(reply_buffer, sizeof(reply_buffer),
                 "[0,%ld,%ld,%lu]", left, right, time);
#else
        snprintf(reply_buffer, sizeof(reply_buffer),
                 "[0,%d,%d,%u]", left, right, time);
#endif

        romiSerial->send(reply_buffer);                
}

void handle_configure(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) string_arg;
        bool success;
        AzhooConfiguration config;

        // encoders
        config.encoder_steps = (uint16_t) args[0];
        config.left_direction = args[7] > 0? 1 : -1;
        config.right_direction = args[8] > 0? 1 : -1;

        // envelope
        config.max_speed = (double) args[1] / 100.0f;
        config.max_acceleration = 0.2; // TODO
        // float max_signal = (float) args[2]; Not used, legacy

        // PI controller
        //int enablePID = args[3]; // Not used, legacy
        config.kp_numerator = args[4];
        config.kp_denominator = 1000;
        config.ki_numerator = args[5];
        config.ki_denominator = 1000;
        //int16_t kd = args[6];  // Not used, legacy
                
        success = azhoo_->configure(config);
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);  
        }
}

void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) string_arg;
        bool success;
        if (args[0] == 0) {
                success = azhoo_->disable();
        } else {
                success = azhoo_->enable();
        }
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);
        }
}

void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) string_arg;
        // TODO: speed is currently multiplied by 100. Should be
        // chnaged to 1000.
        int16_t left = (int16_t) (args[0] * 10);
        int16_t right = (int16_t) (args[1] * 10);
        bool success = azhoo_->set_target_speeds(left, right);
        if (success) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(1, kErrBadState);
        }
}

void handle_stop(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        (void) args;
        (void) string_arg;
        azhoo_->stop();
        romiSerial->send_ok();  
}
