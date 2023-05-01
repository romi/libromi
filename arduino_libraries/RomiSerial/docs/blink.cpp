#include <memory>
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
                throw std::runtime_error("Usage: blink <serial-device>");
        }
        
        std::string device = argv[1];
        auto log = std::make_shared<Console>(); 
        auto serial = std::make_shared<RSerial>(device, 115200, true, log);
        
        RomiSerialClient romiClient(serial, serial, log, 0, "blink");
        nlohmann::json response;
        
        while (true) {
                romiClient.send("L[1]", response);
                delay(1000);
                romiClient.send("L[0]", response);
                delay(1000);
        }
}
