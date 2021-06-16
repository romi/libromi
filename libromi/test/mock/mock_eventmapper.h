#include "gmock/gmock.h"
#include "ui/IJoystickEventMapper.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockEventMapper : public romi::IJoystickEventMapper
{
public:
        MOCK_METHOD(int, map, (romi::IJoystick& joystick, romi::JoystickEvent& event), (override));
};
#pragma GCC diagnostic pop
