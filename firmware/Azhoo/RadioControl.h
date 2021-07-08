
#ifndef _AHZOO_RADIOCONTROL_H
#define _AHZOO_RADIOCONTROL_H

#include "IPWM.h"

class RadioControl : public IPWM
{
public:
        ~RadioControl() override = default;

        int16_t center() override {
                return 1500;
        }
        
        int16_t amplitude() {
                return 500;
        }
};

#endif // _AHZOO_RADIOCONTROL_H
