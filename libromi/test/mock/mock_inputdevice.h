#include "gmock/gmock.h"
#include "api/InputDevice.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockInputDevice : public romi::InputDevice
{
public:
        MOCK_METHOD(int, get_next_event, (), (override));
        MOCK_METHOD(double, get_forward_speed, (), (override));
        MOCK_METHOD(double, get_backward_speed, (), (override));
        MOCK_METHOD(double, get_direction, (), (override));
};
#pragma GCC diagnostic pop
