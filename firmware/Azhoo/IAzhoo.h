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
#ifndef _AZHOO_I_AZHOO_H
#define _AZHOO_I_AZHOO_H

#include <stdint.h>

struct AzhooConfiguration
{
        // Number of pulses per wheel revolution.
        uint16_t encoder_steps;

        // Direction of the encoders:
        // 1: Encoder increments when wheel rolls forwards
        // -1: Encoder decrements when wheel rolls forwards
        int8_t left_direction;
        int8_t right_direction;

        // The maximum speed in rev/s.
        double max_speed;
        
        // The maximum acceleration in rev/s².
        double max_acceleration;

        // The constants Kp and Ki for the PI controller. They are
        // given as fractions (numerator/denominator).
        int16_t kp_numerator;
        int16_t kp_denominator;
        int16_t ki_numerator;
        int16_t ki_denominator;
};

class IAzhoo
{
public:
        virtual ~IAzhoo() = default;
        
        virtual void setup() = 0;
        virtual bool configure(AzhooConfiguration& config) = 0;
        virtual bool enable() = 0;
        virtual bool disable() = 0;
        virtual void stop() = 0;
        
        // Set the new target angular speeds. The left and right
        // values are the angular speed in revolutions/s, multiplied
        // by 1000. So for an angular speed of 0.5 rev/s, the value is
        // 500. Returns true is all went well, and false if the
        // initialization wasn't completed.
        virtual bool set_target_speeds(int16_t left, int16_t right) = 0;

        // Returns true if an update was performed, false otherwise. 
        virtual bool update() = 0;

        virtual void get_encoders(int32_t& left, int32_t& right, uint32_t& time) = 0;

};

#endif // _AZHOO_I_AZHOO_H