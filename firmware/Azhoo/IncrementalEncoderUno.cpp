
#include "IncrementalEncoderUno.h"
        
void IncrementalEncoderUno::init(int32_t pulses_per_revolution,
                                 int32_t increment, 
                                 uint8_t pin_a,
                                 uint8_t pin_b,
                                 EncoderInterruptHandler callback)
{
        IncrementalEncoder::init(pulses_per_revolution, increment);
        pin_b_ = pin_b;
        attachInterrupt(digitalPinToInterrupt(pin_a),
                        callback,
                        RISING);
}
