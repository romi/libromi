#include "gmock/gmock.h"
#include "data_provider/IRomiDeviceData.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockRomiDeviceData : public romi::IRomiDeviceData
{
public:
        MOCK_METHOD(std::string, RomiDeviceType, (), (override));
        MOCK_METHOD(std::string, RomiDeviceHardwareId, (), (override));
};
#pragma GCC diagnostic pop