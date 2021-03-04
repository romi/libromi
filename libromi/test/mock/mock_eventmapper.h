#include "gmock/gmock.h"
#include "JoystickEventMapper.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
class MockEventMapper : public romi::JoystickEventMapper
{
public:
        MOCK_METHOD(int, map, (romi::Joystick& joystick, romi::JoystickEvent& event), (override));
};
#pragma GCC diagnostic pop