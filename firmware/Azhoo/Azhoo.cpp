
#include <Arduino.h>  // TODO
#include "Azhoo.h"

Azhoo::Azhoo(IArduino& arduino, uint32_t interval_millis)
        : arduino_(arduino),
          left_controller_(arduino.left_encoder(),
                           arduino.left_pwm()),
          right_controller_(arduino.right_encoder(),
                            arduino.right_pwm()),
          left_speed_envelope_(),
          right_speed_envelope_(),
          interval_(interval_millis),
          last_time_(0)
{
        left_controller_.debug_ = true; // TODO
}

void Azhoo::setup()
{
        last_time_ = arduino_.milliseconds();
}

void Azhoo::init_encoders(int16_t encoder_steps,
                          int8_t left_increment,
                          int8_t right_increment)
{
        arduino_.init_encoders(encoder_steps, left_increment, right_increment);
        left_controller_.update_encoder_values(interval_ / 1000.0);
        right_controller_.update_encoder_values(interval_ / 1000.0);
}

void Azhoo::init_envelope(double max_acceleration)
{
        left_speed_envelope_.init(max_acceleration, interval_ / 1000.0);
        right_speed_envelope_.init(max_acceleration, interval_ / 1000.0);
}

void Azhoo::init_controllers(int16_t kp_numerator, int16_t kp_denominator,
                             int16_t ki_numerator, int16_t ki_denominator)
{
        left_controller_.init(kp_numerator, kp_denominator,
                              ki_numerator, ki_denominator);
        right_controller_.init(kp_numerator, kp_denominator,
                               ki_numerator, ki_denominator);
}

void Azhoo::set_target_speeds(int16_t left, int16_t right)
{
        left_speed_envelope_.set_target(left);
        right_speed_envelope_.set_target(right);
}
        
bool Azhoo::update()
{
        bool updated = false;
        uint32_t now = arduino_.milliseconds();
        uint32_t dt = now - last_time_;
        
        if (dt >= interval_) {
                int16_t left_speed = left_speed_envelope_.update();
                int16_t right_speed = right_speed_envelope_.update();
                
                // Serial.print(left_speed);
                // Serial.print(" ");
                // Serial.print(right_speed);
                // Serial.println();
                
                left_controller_.update(left_speed);
                right_controller_.update(right_speed);
                last_time_ = now;
                updated = true;
        }
        return updated;
}
