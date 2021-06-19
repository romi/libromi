#include <Arduino.h>
#include "IEncoder.h"

class IncrementalEncoder : public IEncoder
{
protected:
        int a_pin_;
        int b_pin_;
        long direction_;
        double steps_per_rotation_;
        volatile long ticks_;
        unsigned long last_time_;
        long last_ticks_;
        double speed_;
        
        bool read_b_signal() {
                return digitalRead(b_pin_);
        }

        void handle_interrupt() {
                bool b = read_b_signal();
                ticks_ += b ? -direction_ : +direction_;
        }

public:
        IncrementalEncoder(int a_pin, int b_pin, bool forward,
                           double steps_per_rotation);
        ~IncrementalEncoder() override = default;

        void setup() override;
        long get_value() override;
        double get_speed() override;
};
