#ifndef _AHZOO_PICONTROLLER_H
#define _AHZOO_PICONTROLLER_H

#include "IEncoder.h"
#include "IPWM.h"

class PIController
{
public:
        
        IEncoder& encoder_;
        IPWM& pwm_;
        int32_t last_position_;
        int16_t delta_;
        int16_t delta_target_;
        int16_t error_;
        int16_t sum_;
        int16_t p_;
        int16_t i_;
        int16_t out_;
        int16_t pulsewidth_;
        int32_t delta_coeff_;
        int16_t kp_numerator_;
        int16_t kp_denominator_;
        int16_t ki_numerator_;
        int16_t ki_denominator_;
        int16_t max_sum_;
        bool debug_;
        
        PIController(IEncoder& encoder, IPWM& pwm)
                : encoder_(encoder),
                  pwm_(pwm),
                  last_position_(0),
                  delta_(0),
                  delta_target_(0),
                  error_(0),
                  sum_(0),
                  p_(0),
                  i_(0),
                  out_(0),
                  pulsewidth_(0),
                  delta_coeff_(0),
                  kp_numerator_(0),
                  kp_denominator_(1),
                  ki_numerator_(0),
                  ki_denominator_(1),
                  max_sum_(0),
                  debug_(false) {
        }

        void update_encoder_values(double dt) {
                last_position_ = encoder_.get_position();
                
                // delta_target = speed rev/s * encoder_steps steps/rev * dt s;
                // delta_target = speed * 2100 * (20 / 1000);
                // delta_target = vi * 42000 / 1000;
                // delta_target = speed * 42;
                // 16000*20/1000 = 320  
                delta_coeff_ = (int32_t)((double) encoder_.positions_per_revolution() * dt);
        }

        void init(int16_t kp_numerator, int16_t kp_denominator,
                  int16_t ki_numerator, int16_t ki_denominator) {
                kp_numerator_ = kp_numerator;
                kp_denominator_ = kp_denominator;
                ki_numerator_ = ki_numerator;
                ki_denominator_ = ki_denominator;
                if (ki_numerator == 0)
                        max_sum_ = 0;
                else
                        max_sum_ = (int16_t) (ki_denominator * pwm_.amplitude()
                                              / ki_numerator);
        }
        
        void update(int16_t speed) {
                
                int32_t position = encoder_.get_position();

                // Compute the error based on the expected increment
                // of the encoder (delta_target_) and increment of the
                // encoder that was actually measured (delta_).
                delta_ = (int16_t) (position - last_position_);
                delta_target_ = (int16_t) (delta_coeff_ * (int32_t) speed / 1000);
                error_ = (int16_t) (delta_target_ - delta_);

                // Sum the errors for the integral term of the PI
                // controller. To avoid runaway values, we added a
                // safeguard to limit the integral term to a min/max
                // range.
                sum_ = (int16_t) (sum_ + error_);
                if (sum_ > max_sum_)
                        sum_ = max_sum_;
                else if (sum_ < -max_sum_)
                        sum_ = (int16_t) (-max_sum_);

                // Compute the output signal based on the proportional
                // and integral terms. Keep the output inside the
                // acceptable range. The output is directly used to
                // set pulse width of the PWM.
                p_ = (int16_t) (kp_numerator_ * error_ / kp_denominator_);
                i_ = (int16_t) (ki_numerator_ * sum_ / ki_denominator_);
                out_ = (int16_t) (p_ + i_);
                if (out_ < (int16_t) -pwm_.amplitude()) {
                        out_ = (int16_t) (-pwm_.amplitude());
                } else if (out_ > (int16_t) pwm_.amplitude()) {
                        out_ = pwm_.amplitude();
                }


                // Add the center value of the PWM generator to obtain
                // the final pulse width.
                pulsewidth_ = (int16_t) (pwm_.center() + out_);
                pwm_.set(pulsewidth_);

                if (debug_) {  // TODO
                        Serial.print(millis());
                        Serial.print(" ");
                        Serial.print(speed);
                        Serial.print(" ");
                        Serial.print(delta_target_);
                        Serial.print(" ");
                        Serial.print(delta_);
                        Serial.print(" ");
                        Serial.print(error_);
                        Serial.print(" ");
                        Serial.print(sum_);
                        Serial.print(" ");
                        Serial.print(out_);
                        Serial.print(" ");
                        Serial.print(pulsewidth_);
                        Serial.println();
                }
                
                last_position_ = position;
        }
};

#endif // _AHZOO_PICONTROLLER_H
