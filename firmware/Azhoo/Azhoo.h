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
#ifndef _AZHOO_AZHOO_H
#define _AZHOO_AZHOO_H

#include "IAzhoo.h"
#include "IArduino.h"
#include "SpeedEnvelope.h"
#include "PIController.h"

class Azhoo : public IAzhoo
{
public:
        static const uint32_t kDefaultUpdateInterval;

        /*
          States: [created] -> [set-up] -> [configured] -> [enabled] <-> [disabled]
        */
        enum AzhooState {
                kCreated = 0,
                kSetUp,
                kConfigured,
                kEnabled,
                kDisabled
        };
        
        IArduino& arduino_;

public:
        // FIXME: made public for unit testing
        PIController left_controller_;
        PIController right_controller_;
        SpeedEnvelope left_speed_envelope_;
        SpeedEnvelope right_speed_envelope_;
        uint32_t interval_;
        uint32_t last_time_;
        AzhooState state_;

        bool is_set_up();
        bool is_configured();
        bool is_enabled();
        bool is_disabled();
        
protected:
        
        void init_encoders(uint16_t encoder_steps,
                           int8_t left_direction,
                           int8_t right_direction);

        void init_speed_envelope(double max_speed, double max_acceleration);

        void init_pi_controllers(int16_t kp_numerator, int16_t kp_denominator,
                                 int16_t ki_numerator, int16_t ki_denominator);

public:
        
        Azhoo(IArduino& arduino, uint32_t interval_millis);
        ~Azhoo() override = default;
        
        void setup() override;
        bool configure(AzhooConfiguration& config) override;
        bool enable() override;
        bool disable() override;
        
        // Set the new target angular speeds. The left and right
        // values are the angular speed in revolutions/s, multiplied
        // by 1000. So for an angular speed of 0.5 rev/s, the value is
        // 500. Returns true is all went well, and false if the
        // initialization wasn't completed.
        bool set_target_speeds(int16_t left, int16_t right) override;

        // Returns true if an update was performed, false otherwise. 
        bool update() override;

        // Stops the rover immediately.
        void stop() override;

        void get_encoders(int32_t& left, int32_t& right, uint32_t& time) override;
};

#endif // _AZHOO_AZHOO_H
