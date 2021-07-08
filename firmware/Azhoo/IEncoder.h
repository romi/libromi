
#ifndef _AHZOO_I_ENCODER_H
#define _AHZOO_I_ENCODER_H

#include <stdint.h>

class IEncoder
{
public:
        virtual ~IEncoder() = default;

        virtual void init(int16_t pulses_per_revolution, int8_t direction) = 0;
        virtual int32_t get_position() = 0;
        virtual int32_t positions_per_revolution() = 0;
};

#endif // _AHZOO_I_ENCODER_H
