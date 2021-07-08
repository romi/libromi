
#ifndef _AHZOO_SPEEDENVELOPE_H
#define _AHZOO_SPEEDENVELOPE_H

class SpeedEnvelope
{
public:
        
        int16_t target_;
        int16_t current_;
        int16_t increment_;
        
        SpeedEnvelope()
                : target_(0),
                  current_(0),
                  increment_(0) {
        }
        
        void init(double acceleration, double dt) {
                double dv = acceleration * dt;
                increment_ = (int16_t) (dv * 1000.0);
        }

        void set_target(int16_t target) {
                target_ = target;
        }

        int16_t update() {
                if (current_ < target_) {
                        current_ = (int16_t) (current_ + increment_);
                        if (current_ > target_)
                                current_ = target_;
                } else if (current_ > target_) {
                        current_ = (int16_t) (current_ - increment_);
                        if (current_ < target_)
                                current_ = target_;
                }
                return current_;
        }
};

#endif // _AHZOO_SPEEDENVELOPE_H
