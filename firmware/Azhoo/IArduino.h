
#ifndef _AHZOO_I_ARDUINO_H
#define _AHZOO_I_ARDUINO_H

#include "IEncoder.h"
#include "IPWM.h"

class IArduino
{
public:
        virtual ~IArduino() = default;
        virtual IEncoder& left_encoder() = 0;
        virtual IEncoder& right_encoder() = 0;
        virtual IPWM& left_pwm() = 0;
        virtual IPWM& right_pwm() = 0;
        virtual uint32_t milliseconds() = 0;

        virtual void init_encoders(int16_t encoder_steps,
                                   int8_t left_increment,
                                   int8_t right_increment) = 0;
};

#endif // _AHZOO_I_ARDUINO_H
