#include <memory>
#include <iostream>
#include <unistd.h>
#include <RomiSerialClient.h>
#include <RSerial.h>
#include <Console.h>

using namespace romiserial;

void delay(size_t milliseconds)
{
        usleep(milliseconds * 1000);
}

int main(int argc, char **argv)
{
        if (argc < 2) {
                throw std::runtime_error("Usage: analogread_app <serial-device>");
        }
        
        std::string device = argv[1];
        auto log = std::make_shared<Console>(); 
        auto serial = std::make_shared<RSerial>(device, 115200, true, log);
        
        RomiSerialClient romiClient(serial, serial, log, 0, "analogread");
        nlohmann::json response;
        
        while (true) {
                romiClient.send("A", response);
                std::cout << "Sensor value: " << response[1] << std::endl;
                delay(1000);
        }
}
