#include <ArduinoSerial.h>
#include <RomiSerial.h>

using namespace romiserial;

void handle_led(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);

const static MessageHandler handlers[] = {
        { 'L', 1, false, handle_led },
};

ArduinoSerial serial(Serial);
RomiSerial romiSerial(serial, serial, handlers, sizeof(handlers) / sizeof(MessageHandler));

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    romiSerial.handle_input();
}

void handle_led(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
    if (args[0] == 0) {
        digitalWrite(LED_BUILTIN, LOW);
    } else {
        digitalWrite(LED_BUILTIN, HIGH);
    }
    romiSerial->send_ok();
}
