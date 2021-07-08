#include "ArduinoUno.h"
#include "Azhoo.h"

static const unsigned long kUpdateInterval = 20;

ArduinoUno arduino_;
Azhoo azhoo_(arduino_, kUpdateInterval);

#if 1
double encoder_steps = 2100.0;
double max_acceleration = 0.2;
int32_t KpNumerator = 1400;
int32_t KpDenominator = 100;
int32_t KiNumerator = 7;
int32_t KiDenominator = 10;
        
#else
double encoder_steps = 16000.0;
double max_acceleration = 0.2;
int32_t KpNumerator = 7;
int32_t KpDenominator = 100;
int32_t KiNumerator = 1;
int32_t KiDenominator = 1;
#endif

void setup()
{
        Serial.begin(115200);
        while (!Serial)
                ;
        
        arduino_.setup();
        azhoo_.setup();

        //
        
        azhoo_.init_encoders(encoder_steps, -1, 1);
        azhoo_.init_speed_envelope(max_acceleration);
        azhoo_.init_pi_controllers(KpNumerator, KpDenominator, KiNumerator, KiDenominator);
}

void loop()
{
        //azhoo_.update();
        test1();
}

void test1()
{
        int16_t counter = 0;

        counter = 0;
        azhoo_.set_target_speeds(500, 500);
        while (true) {
                if (azhoo_.update()) {
                        counter++;
                        if (counter == 400)
                                break;
                }
        }

        counter = 0;
        azhoo_.set_target_speeds(0, 0);
        while (true) {
                if (azhoo_.update()) {
                        counter++;
                        if (counter == 400)
                                break;
                }
        }

        counter = 0;
        azhoo_.set_target_speeds(-500, -500);
        while (true) {
                if (azhoo_.update()) {
                        counter++;
                        if (counter == 400)
                                break;
                }
        }

        counter = 0;
        azhoo_.set_target_speeds(0, 0);
        while (true) {
                if (azhoo_.update()) {
                        counter++;
                        if (counter == 400)
                                break;
                }
        }
}

