
#ifndef _AHZOO_I_PWM_H
#define _AHZOO_I_PWM_H

class IPWM
{
public:
        virtual ~IPWM() = default;
        virtual int16_t center() = 0;
        virtual int16_t amplitude() = 0;
        virtual void set(int16_t pulse_width) = 0;
};

#endif // _AHZOO_I_PWM_H
