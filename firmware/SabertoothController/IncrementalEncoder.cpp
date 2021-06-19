#include "IncrementalEncoder.h"

static void IncrementalEncoder_handle_interrupt();

IncrementalEncoder::IncrementalEncoder(int a_pin, int b_pin, bool forward,
                                       double steps_per_rotation)
        : a_pin_(a_pin),
          b_pin_(b_pin),
          direction_(0),
          steps_per_rotation_(steps_per_rotation),
          ticks_(0),
          last_time_(0),
          last_ticks_(0),
          speed_(0)
{
        direction_ = forward? -1 : 1;
}

void IncrementalEncoder::setup()
{
        pinMode(a_pin_, INPUT_PULLUP);
        pinMode(b_pin_, INPUT_PULLUP);
        
        attachInterrupt(digitalPinToInterrupt(a_pin_),
                        IncrementalEncoder_handle_interrupt,
                        RISING);
}
 
static void IncrementalEncoder_handle_interrupt()
{
        //encoder->handle_interrupt();
}

long IncrementalEncoder::get_value()
{
        return ticks_;
}

double IncrementalEncoder::get_speed()
{
        unsigned long t = millis();
        
        if (t > last_time_ + 20) {
                float dt = (t - last_time_) / 1000.0;
                float w = (ticks_ - last_ticks_) / dt / steps_per_rotation_;
                
                speed_ = 0.2 * w + 0.8 * speed_;

                last_ticks_ = ticks_;
                last_time_ = t;
        }

        return speed_;
}
