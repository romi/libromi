
#ifndef _AZHOO_AZHOO_H
#define _AZHOO_AZHOO_H

#include "IArduino.h"
#include "SpeedEnvelope.h"
#include "PIController.h"

class Azhoo
{
protected:

        IArduino& arduino_;
        PIController left_controller_;
        PIController right_controller_;
        SpeedEnvelope left_speed_envelope_;
        SpeedEnvelope right_speed_envelope_;
        unsigned long interval_;
        unsigned long last_time_;
        
public:
        
        Azhoo(IArduino& arduino, unsigned long interval_millis);
        ~Azhoo() = default;

        void setup();
        
        void init_encoders(int16_t encoder_steps,
                           int8_t left_increment,
                           int8_t right_increment);

        // The maximum acceleration in rev/s².
        void init_envelope(double max_acceleration);

        // The controller constants Kp and Ki as fractions
        // (numerator/denominator).
        void init_controllers(int16_t kp_numerator, int16_t kp_denominator,
                              int16_t ki_numerator, int16_t ki_denominator);

        // Set the new target angular speeds. The left and right
        // values are the angular speed in revolutions/s, multiplied
        // by 1000. So for an angular speed of 0.5 rev/s, the value is
        // 500.
        void set_target_speeds(int16_t left, int16_t right);

        // Returns true if an update was performed, false otherwise. 
        bool update();
};

#endif // _AZHOO_AZHOO_H
