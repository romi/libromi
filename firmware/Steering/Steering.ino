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
#include "block.h"
#include "stepper.h"
#include "config.h"
#include "ArduinoUno.h"
#include <ArduinoSerial.h>
#include <RomiSerial.h>

using namespace romiserial;
/**
 *  \brief The possible states of the controller (the main thread).
 */
enum {
        STATE_RUNNING = 'r',
        STATE_PAUSED = 'p',
        STATE_HOMING = 'h',
        STATE_ERROR = 'e'
};

extern volatile block_t *current_block;

uint8_t controller_state;

static const char *kInvalidState = "Invalid state";

void handle_moveto(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_move(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_pause(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_continue(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_reset(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_zero(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_position(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_idle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_print(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);
void handle_test(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);

const static MessageHandler handlers[] = {
        { 'm', 3, false, handle_moveto },
        { 'M', 3, false, handle_move },
        { 'V', 2, false, handle_moveat },
        { 'p', 0, false, handle_pause },
        { 'c', 0, false, handle_continue },
        { 'r', 0, false, handle_reset },
        { 'z', 0, false, handle_zero },
        { 'P', 0, false, send_position },
        { 'I', 0, false, send_idle },
        { 'H', 0, false, handle_homing },
        { 'E', 1, false, handle_enable },
        { 'D', 1, false, handle_print },
        { 'T', 1, false, handle_test },
        { '?', 0, false, send_info },
};

ArduinoUno arduino_;
ArduinoSerial serial(Serial);
RomiSerial romiSerial(serial, serial, handlers, sizeof(handlers) / sizeof(MessageHandler));

static char reply_string[80];

int moveat(int dx, int dy);

void reset()
{
        block_buffer_clear();
        stepper_reset();
}

void setup()
{
        disable_driver();
        
        Serial.begin(115200);
        while (!Serial)
                ;
        
        arduino_.setup();
        arduino_.init_encoders(1024, 1, -1);

        init_block_buffer();
        init_pins();
        init_stepper();

        controller_state = STATE_RUNNING;
        enable_stepper_timer();
}

static unsigned long last_time = 0;
static unsigned long last_print_time = 0;
static int16_t id = 0;
static bool print_ = false;

void loop()
{
        romiSerial.handle_input();
        delay(1);

        if (print_) {
                print_status();
                delay(50);
        }
}

void print_status()
{
        Serial.print(arduino_.left_encoder().get_position());
        if (arduino_.left_encoder().get_index()) {
                Serial.print("***");
                arduino_.left_encoder().reset_index();
        }
        Serial.print("    ");
        Serial.print(arduino_.right_encoder().get_position());
        if (arduino_.right_encoder().get_index()) {
                Serial.print("***");
                arduino_.right_encoder().reset_index();
        }
        Serial.println();
}

int moveat(int dx, int dy)
{
        int err = 0;
        block_t *block = block_buffer_get_empty();
        if (block == 0) {
                err = -1;
        } else if (dx == 0 && dy == 0) {
                reset();
        } else {
                block->type = BLOCK_MOVEAT;
                block->data[DT] = 1000;
                block->data[DX] = dx;
                block->data[DY] = dy;
                block_buffer_ready();
        }
        return err;
}

int move(int dt, int dx, int dy)
{
        int err = 0;
        block_t *block = block_buffer_get_empty();
        if (block == 0) {
                err = -1;
        } else {
                block->type = BLOCK_MOVE;
                block->data[DT] = dt;
                block->data[DX] = dx;
                block->data[DY] = dy;
                block_buffer_ready();
        }
        return err;
}

void handle_moveto(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_RUNNING
            || controller_state == STATE_PAUSED) {
                if (args[0] > 0) {
                        block_t *block = block_buffer_get_empty();
                        if (block == 0) {
                                romiSerial->send_error(1, "Again");  
                        } else {
                                block->type = BLOCK_MOVETO;
                                block->data[DT] = args[0];
                                block->data[DX] = args[1];
                                block->data[DY] = args[2];
                                block_buffer_ready();
                                romiSerial->send_ok();  
                        }
                } else {
                        romiSerial->send_error(100, "Invalid DT");  
                }
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_move(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_RUNNING
            || controller_state == STATE_PAUSED) {
                if (args[0] > 0) {
                        if (move(args[0], args[1], args[2]) == 0) {
                                romiSerial->send_ok();  
                        } else {
                                romiSerial->send_error(1, "Again");  
                        }
                } else {
                        romiSerial->send_error(100, "Invalid DT");                  
                }
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_moveat(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_RUNNING
            || controller_state == STATE_PAUSED) {

                if (moveat(args[0], args[1]) == 0) {
                        romiSerial->send_ok();  
                } else {
                        romiSerial->send_error(1, "Again");  
                }
                
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_pause(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_RUNNING) {
                disable_stepper_timer();
                controller_state = STATE_PAUSED;
                romiSerial->send_ok();  
        } else if (controller_state == STATE_PAUSED) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_continue(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED) {
                controller_state = STATE_RUNNING;
                enable_stepper_timer();
                romiSerial->send_ok();  
        } else if (controller_state == STATE_RUNNING) {
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_reset(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED) {
                controller_state = STATE_RUNNING;
                reset();
                enable_stepper_timer();
                romiSerial->send_ok();  
        } else if (controller_state == STATE_RUNNING) {
                reset();
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void handle_zero(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (controller_state == STATE_PAUSED || is_idle()) {
                stepper_zero();
                arduino_.left_encoder().set_zero();
                arduino_.right_encoder().set_zero();
                romiSerial->send_ok();  
        } else {
                romiSerial->send_error(101, kInvalidState);  
        }
}

void send_position(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        int32_t pos[3];
        get_stepper_position(pos);

        snprintf(reply_string, sizeof(reply_string),
                 "[0,%ld,%ld,%ld]", pos[0], pos[1], pos[2]);
        
        romiSerial->send(reply_string); 
}

static inline bool is_idle()
{
        return ((controller_state == STATE_RUNNING)
                && stepper_is_idle());
}

static inline void wait()
{
        while (!is_idle()) {
                romiSerial.handle_input();
                delay(1);
        }
}

void send_idle(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        snprintf(reply_string, sizeof(reply_string), "[0,%d,\"%c\"]",
                 is_idle(), controller_state);
        romiSerial->send(reply_string); 
}

bool seek_index(int axis, int speed, int max_steps, IEncoder& encoder)
{
        bool found = false;
        int32_t start = encoder.get_position();
        int32_t end;
        int8_t direction;

        encoder.reset_index();
        
        if (speed > 0) {
                direction = 1;
                end = start + max_steps;
        } else {
                direction = -1;
                end = start - max_steps;
        }
        
        if (axis == 0)
                moveat(speed, 0);
        else
                moveat(0, speed);
        
        while (true) {
                romiSerial.handle_input();
                
                int32_t current = encoder.get_position();
                
                // Serial.print(axis);
                // Serial.print(": ");
                // Serial.print(current);
                // Serial.print(" ");
                // Serial.print(start);
                // Serial.print(" ");
                // Serial.print(end);
                // Serial.print(" ");
                // Serial.println(direction);
                
                if ((direction > 0 && current > end)
                    || (direction < 0 && current < end)) {
                        break;
                } else if (encoder.get_index()) {
                        found = true;
                        break;
                }
                delay(10);
        }

        moveat(0, 0);
        return found;
}

bool do_homing_axis(int axis, IEncoder& encoder)
{
        bool success = false;
        uint16_t steps = encoder.positions_per_revolution();
        // steps/16 = 22.5°
        // steps/180 = 2°
        if (seek_index(axis, -500, steps/180, encoder)
            || seek_index(axis, 500, steps/16, encoder)
            || seek_index(axis, -500, steps+1, encoder)) {
                success = true;
                encoder.set_zero();
                stepper_zero();
        }
        return success;
}

bool do_homing()
{
        return (do_homing_axis(0, arduino_.left_encoder())
                && do_homing_axis(1, arduino_.right_encoder()));
}

void handle_homing(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // Stop whatever is ongoing
        reset();

        // Make sure the timer is running
        enable_stepper_timer();

        controller_state = STATE_HOMING;

        // Reply the client immediately because there's a timeout on
        // sending a response.
        romiSerial->send_ok();
        
        if (do_homing()) {
                reset();
                stepper_zero();
                controller_state = STATE_RUNNING;
        } else {
                controller_state = STATE_ERROR;
        }
}

void handle_enable(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (args[0] == 0) {
                disable_driver();
        } else {
                enable_driver()
        }
        romiSerial->send_ok();
}

void handle_print(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        if (args[0] == 0) {
                print_ = false;
        } else {
                print_ = true;
        }
        romiSerial->send_ok();
}

void send_info(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        romiSerial->send("[0,\"Steering\",\"0.1\",\"" __DATE__ " " __TIME__ "\"]"); 
}

// static bool quit_testing;

// void start_test()
// {
//         quit_testing = false;
//         while (!quit_testing) {
                
//                 moveat(1000, 1000, 100);
//                 delay(500);
                
//                 moveat(-1000, -1000, -100);
//                 delay(500);
                
//                 update_zero_switches();
//                 Serial.print("#![");
//                 Serial.print(limit_switches[0]);
//                 Serial.print(',');
//                 Serial.print(limit_switches[1]);
//                 Serial.print(',');
//                 Serial.print(limit_switches[2]);
//                 Serial.print("]:xxxx\r\n");

//                 romiSerial.handle_input();
//         }
// }

// void stop_test()
// {
//         quit_testing = true;
//         moveat(0, 0, 0);
// }

void handle_test(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
        // romiSerial->send_ok();
        // if (args[0] == 0) {
        //         stop_test();
        // } else {
        //         start_test();
        // }
}
