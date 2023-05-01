#include <ArduinoSerial.h>
#include <RomiSerial.h>

using namespace romiserial;

ArduinoSerial serial(Serial);

void read_sensor(IRomiSerial *romiSerial, int16_t *args, const char *string_arg);

const static MessageHandler handlers[] = {
        { 'A', 0, false, read_sensor },
};

RomiSerial romiSerial(serial, serial, handlers, sizeof(handlers) / sizeof(MessageHandler));

void setup() {
    Serial.begin(115200);
}

void loop() {
    romiSerial.handle_input();
}

void read_sensor(IRomiSerial *romiSerial, int16_t *args, const char *string_arg)
{
    char reply[16];
    int sensorValue = analogRead(A0);
    snprintf(reply, sizeof(reply), "[0,%d]", sensorValue);
    romiSerial->send(reply); 
}
