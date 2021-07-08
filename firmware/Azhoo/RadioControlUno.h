
#ifndef _AHZOO_RADIOCONTROLUNO_H
#define _AHZOO_RADIOCONTROLUNO_H

#include "RadioControl.h"

class RadioControlUno : public RadioControl
{
protected:
        volatile uint16_t* ocr_;
        
public:
        RadioControlUno() : ocr_(nullptr) {}
        
        ~RadioControlUno() override = default;

        void init(volatile uint16_t* ocr) {
                ocr_ = ocr;
        }

        void set(int16_t pulsewidth) override {
                *ocr_ = (uint16_t) (2 * pulsewidth);
        }
};

#endif // _AHZOO_RADIOCONTROLUNO_H
