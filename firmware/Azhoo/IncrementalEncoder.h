
#ifndef _AHZOO_INCREMENTAL_ENCODER_H
#define _AHZOO_INCREMENTAL_ENCODER_H

#include "IEncoder.h"

class IncrementalEncoder : public IEncoder
{
public:
        volatile int32_t position_;
        int32_t increment_;
        // https://www.cuidevices.com/blog/what-is-encoder-ppr-cpr-and-lpr
        int32_t pulses_per_revolution_;

        IncrementalEncoder()
                : position_(0),
                  increment_(1),
                  pulses_per_revolution_(1) {
        }
        
        ~IncrementalEncoder() override = default;
        
        void init(int16_t pulses_per_revolution, int8_t direction) override {
                init((int32_t) pulses_per_revolution, (int32_t) direction);
        }
        
        void init(int32_t pulses_per_revolution, int32_t increment) {
                pulses_per_revolution_ = pulses_per_revolution;
                increment_ = increment;
                position_ = 0;
        }

        int32_t get_position() override {
                return position_;
        }

        int32_t positions_per_revolution() override {
                return pulses_per_revolution_;
        }
        
        inline void increment() {
                position_ += increment_;
        }
        
        inline void decrement() {
                position_ -= increment_;
        }

        // For testing only
        void set_position(int32_t t) {
                position_ = t;
        }
};

#endif // _AHZOO_INCREMENTAL_ENCODER_H
